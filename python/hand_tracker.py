import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision
import socket
import json
import joblib
import math
from collections import deque, Counter
history = deque(maxlen=8)

def vec_sub(a, b):
    return (a[0]-b[0], a[1]-b[1], a[2]-b[2])


def vec_add(a, b):
    return (a[0]+b[0], a[1]+b[1], a[2]+b[2])


def vec_div(a, s):
    return (a[0]/s, a[1]/s, a[2]/s)


def dot(a, b):
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]


def length(v):
    return math.sqrt(dot(v, v))


def cross(a, b):
    return (
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    )


def finger_bend(landmarks, mcp, pip, tip):
    v1 = vec_sub(landmarks[pip], landmarks[mcp])
    v2 = vec_sub(landmarks[tip], landmarks[pip])

    l = length(v1) * length(v2)

    if l == 0:
        return 0

    return dot(v1, v2) / l


def thumb_distance(landmarks):
    palm_center = vec_div(vec_add(landmarks[5], landmarks[17]), 2.0)
    diff = vec_sub(landmarks[4], palm_center)
    return length(diff)


def extract_features(hand):

    # convert mediapipe landmarks
    landmarks = [(lm.x, lm.y, lm.z) for lm in hand]

    features = []

    # Thumb distance
    features.append(thumb_distance(landmarks))

    # Thumb to Index
    features.append(length(vec_sub(landmarks[4], landmarks[8])))

    # Finger bends
    features.append(finger_bend(landmarks, 5, 6, 8))   # index
    features.append(finger_bend(landmarks, 9, 10, 12)) # middle
    features.append(finger_bend(landmarks, 13, 14, 16))# ring
    features.append(finger_bend(landmarks, 17, 18, 20))# pinky
    features.append(finger_bend(landmarks, 2, 3, 4))   # thumb

    # Finger spreads
    features.append(length(vec_sub(landmarks[8],  landmarks[12])))
    features.append(length(vec_sub(landmarks[12], landmarks[16])))
    features.append(length(vec_sub(landmarks[16], landmarks[20])))

    # Wrist distances
    features.append(length(vec_sub(landmarks[8],  landmarks[0])))
    features.append(length(vec_sub(landmarks[12], landmarks[0])))

    # Palm orientation
    normal = cross(
        vec_sub(landmarks[5], landmarks[0]),
        vec_sub(landmarks[17], landmarks[0])
    )

    features.append(normal[2])
    features.append(normal[1])
    features.append(normal[0])

    return features

# load trained KNN model
model = joblib.load("knn_model.pkl")

# cv2 OpenCV lib to open camera and show the window and draw the lines
# mediapipe ready-made computer-vision framwork, contains pretrained models aka hands faces pose ....
# note that mediapipe works on images (pixel data from each frame)
# the tasks part + vision is the api for the hand landmark
# TCP comms to send data to C++

# Webcam → MediaPipe Model → 21 Landmarks → Feature Extraction → KNN Prediction → Send Result to C++

MODEL_PATH = "hand_landmarker.task"

HOST = "127.0.0.1"
PORT = 3000

# create IPv4 TCP protocol socket and connect it c++ server
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

# loading the model and with one hand configuration
base_options = python.BaseOptions(model_asset_path=MODEL_PATH)
options = vision.HandLandmarkerOptions(
    base_options=base_options,
    num_hands=1
)

# now load it into memory
landmarker = vision.HandLandmarker.create_from_options(options)

# open default cam
cap = cv2.VideoCapture(0)

# connections between landmarks
CONNECTIONS = [
    (0,1),(1,2),(2,3),(3,4),  #thump
    (0,5),(5,6),(6,7),(7,8), # finger 2
    (5,9),(9,10),(10,11),(11,12), # 3
    (9,13),(13,14),(14,15),(15,16), # 4
    (13,17),(17,18),(18,19),(19,20), # 5
    (0,17) #center of palm hand
]


while True:  #exit only when ESC pressed

    # each time camera return frame we check it
    ret, frame = cap.read()
    if not ret: #no frame we are done
        break

    h, w, _ = frame.shape # get the frame shape for the model (only height and width cuz landmarks are normalized )

    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # convert from BGR to RGB format cuz mediapipe uses RGB and openCV BGR
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb) # convert to image that mediapipe accepts

    result = landmarker.detect(mp_image) # run the neural network model

    if result.hand_landmarks: # if hand detected

        for hand in result.hand_landmarks: # process each hand detected (only 1 :D )

            points = []

            for i, lm in enumerate(hand): # hand contains 21 landmarks objects and each landmark has x y and z from 0 to 1 (normalized to image which means relative points to image size )

                px = int(lm.x * w) # convert to pixel cords
                py = int(lm.y * h)

                points.append((px, py)) # save so we can draw it

                # draw point
                cv2.circle(frame, (px, py), 5, (0,255,0), -1) # (place, center cord, radius, color, filled circle)

                # Draw landmark index number
                cv2.putText(
                    frame,
                    f"{i}",
                    (px + 5, py - 5),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (0, 0, 255),
                    1,
                    cv2.LINE_AA
                )

            # draw connections
            for c in CONNECTIONS:
                cv2.line(frame, points[c[0]], points[c[1]], (255,0,0), 2)

            # compute ML features
            features = extract_features(hand)

            prediction = model.predict([features])[0]

            history.append(prediction)

            stable = Counter(history).most_common(1)[0][0]

            print("Prediction:", stable)

            sock.sendall((stable + "\n").encode())

    cv2.imshow("Hand Skeleton", frame) #display the frame

    if cv2.waitKey(1) & 0xFF == 27:  # exit on ESC
        break

cap.release()
sock.close()
cv2.destroyAllWindows()