import os

def delete_xml_files(directory):
    # 指定したディレクトリ内のファイルとフォルダを走査
    for filename in os.listdir(directory):
        # ファイルパスを作成
        file_path = os.path.join(directory, filename)
        # .xmlファイルかどうかをチェック
        if filename.endswith(".xml"):
            try:
                # ファイルを削除
                os.remove(file_path)
                print(f"Deleted: {file_path}")
            except Exception as e:
                print(f"Failed to delete {file_path}: {e}")

# 使用例：カレントディレクトリ内の.xmlファイルを削除
delete_xml_files(".")
