import requests
import gzip
import zlib
from io import BytesIO

# URL of the website/resource you want to request
url = "https://code.jquery.com/jquery-3.7.1.min.js" # 긴 텍스트인데

# Send an HTTP GET request with the "Accept-Encoding" header specifying support for Gzip and Deflate compression
headers = {"Accept-Encoding": "gzip, deflate"}
response = requests.get(url, headers=headers)

print("step1 - print http response header to find Content-Encoding: gzip")
print(response.headers)
print("==============================================")



print("step2 - decompress gzip")

# Check if the response is compressed and handle accordingly
if response.headers.get("Content-Encoding") == "gzip":
    try:
        # If the response is Gzip compressed, attempt to decompress it using the gzip module
        decompressed_data = gzip.decompress(response.content)
    except gzip.BadGzipFile:
        print("error!: BadGzipFile!")
        print("")

        # If it's not a valid Gzip file, use the content as is
        decompressed_data = response.content
        byte_size = len(decompressed_data)

        print("byte size: ", byte_size, "bytes")
        print("http response header에 Content-Length: 30336인데 실제 byte size는 87000인걸 보면, gzip의 압축률이한 60~70% 정도 되나보다.")
        # print(decompressed_data.decode("utf-8"))  # Assuming the content is text (adjust decoding as needed)
