import serial
import time
from multiprocessing import Value, Array, Process

#シリアル通信を行う

#シリアル通信をしてconstOpenかどうかを見ておく　＞constOpenならそれぞれの実行、constOpenからretがでるまで物体検出からの指示を無視する。
#物体検出からの共有配列から扉の開き閉めをシリアル通信で行う。
ser = serial.Serial('COM10', 115200, timeout=1)

def run(categorie_names):    
    line = ser.readline().decode('utf-8').strip()
    if line == 'constOpen':
      run = True
      while run:
        line = ser.readline().decode('utf-8').strip()
        if line == 'ret':
          run = False
          
    
    #categorie_namesが3回同じ物体を検出したとき命令を出す
    if len(categorie_names) > 3 and categorie_names[4] != 'reset':

      if categorie_names[0].decode('utf-8') == categorie_names[1].decode('utf-8') \
        and categorie_names[1].decode('utf-8') == categorie_names[2].decode('utf-8'):
        
        send_message('M2close')
        send_message(categorie_names[0].decode('utf-8') + 'M1')
        # バイト列として共有メモリをリセット
        categorie_names[:] = b'\x00' * len(categorie_names)
        #仕分け部分と同時に箱の開閉を行ったとき塞ぐのが間に合わないかもしれないため0.5秒待つ。この時間については要検討
        time.sleep(0.5)
        send_message('M2open')

        #categorie_namesの4番目に適当な文字を入れて消去してもいいことを示す。
        #現在raspberrypiに書いているコードはすべての条件分岐を書いているのでそのそれぞれの分岐の後で次のように書く
        categorie_names[4] = 'reset'

        


if __name__ == '__main__':
  run()

