import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision
import socket
import json

# cv2 OpenCV lib to open camera and show the window and draw the lines 
# mediapipe ready-made computer-vision framwork, contains pretrained models aka hands faces pose .... 
# note that mediapipe works on images (pixel data from each frame)
# the tasks part + vision is the api for the hand landmark  
# TCP comms to send data to C++ 

#Webcam → MediaPipe Model → 21 Landmarks → Draw Skeleton → Send JSON via TCP → C++ Program

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

# now load it into memeory 
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

while True:  #exit only when ESP pressed 
    # each time camera return frame we check it  
    ret, frame = cap.read()
    if not ret: #no frame we are done
        break

    h, w, _ = frame.shape # get the frame shape for the model (only height and width cuz landmarks are normalized ) 

    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # convert from BGR to RGB format cuz mediapipe uses RGB and openCV BGR
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb) # convert to image that mediapipe accepts 

    result = landmarker.detect(mp_image) # run the neural network model 

    if result.hand_landmarks: # if hand deteceted 
        for hand in result.hand_landmarks: # process each hand detected (only 1 :D )
            vector = []
            points = []

            for i, lm in enumerate(hand): # hand contains 21 landmarks objects and each landmark has x y and z from 0 to 1 (normalized to image which means relative points to image size )
                vector.extend([lm.x, lm.y, lm.z]) # vector of 63 length (3 for each 21)
                px = int(lm.x * w) # convert to pixel cords 
                py = int(lm.y * h)
                points.append((px, py)) # save so we can draw it 

                # draw point
                cv2.circle(frame, (px, py), 5, (0,255,0), -1) # (palce, center cord, radius, color, filled circle)
		
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

            # send to C++
            data = json.dumps(vector) + "\n" #conver to json,  \n so we can read by lines in C++ :D 
            sock.sendall(data.encode()) # send it :D

    cv2.imshow("Hand Skeleton", frame) #display the frame 

    if cv2.waitKey(1) & 0xFF == 27:  # exit on ESC 
        break

cap.release() 
sock.close()
cv2.destroyAllWindows()