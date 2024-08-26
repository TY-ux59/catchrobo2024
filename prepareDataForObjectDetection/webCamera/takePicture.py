import cv2 #openCV
import shutil
import os

#vscodeでprepareDataのフォルダのディレクトリを選択する必要がある。
folderPath = 'objectDetection/images/images'
def count_files_in_folder(folder_path):
    #フォルダ内のファイル数をカウント
    file_count = len([f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))])
    return file_count    

#もしここでエラーが出るならばカメラの番号を確認する必要がある（1の部分）
cap = cv2.VideoCapture(1) 
i = count_files_in_folder(folder_path=folderPath) + 1

while True: #永久ループ
    x = input()
    ret, frame = cap.read()#画像取得

    cv2.imwrite(f"{i}.jpg", frame)
    shutil.move(f'{i}.jpg', "objectDetection//images//images//")
    i += 1




        



