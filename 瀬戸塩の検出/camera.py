import cv2 #openCV
import shutil

cap = cv2.VideoCapture(1) 
i = 938

while True: #永久ループ
    x = input()
    ret, frame = cap.read()#画像取得

    cv2.imwrite(f"{i}.jpg", frame)
    shutil.move(f'{i}.jpg', "images//images//")
    i += 1
