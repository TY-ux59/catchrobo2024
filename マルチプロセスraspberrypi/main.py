import ObjectDetection
import SerialNet
from multiprocessing import Value, Array, Process

def Serialnet(categorie_names):
    SerialNet.run(categorie_names)

def Objectdetection(categorie_names):
    ObjectDetection.main(categorie_names)

if __name__ == '__main__':
    categorie_names = Array('c', 4)

    p1 = Process(target=Serialnet, args=(categorie_names,))
    p2 = Process(target=Objectdetection, args=(categorie_names,))

    # プロセスを開始
    p1.start()
    p2.start()

    # すべてのプロセスが終了するまで待機
    p1.join()
    p2.join()     