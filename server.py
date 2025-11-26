from flask import Flask
from flask_cors import CORS
import requests

app = Flask(__name__)
CORS(app)

@app.route('/bus')
def bus():
    arsId = "50340"   # 🔥 한남대학교 정문 정류장 ARS 번호
    serviceKey = "681466b15b0f1baf0cab90025708a28ca91d75f1a750cfad4dafb1c7698948e1"  # 🔥 너의 인증키 그대로 넣기
    
    url = f"http://openapitraffic.daejeon.go.kr/api/rest/arrive/getArrInfoByUid?arsId={arsId}&serviceKey={serviceKey}"

    xml = requests.get(url).text
    return xml

app.run(port=3000)
