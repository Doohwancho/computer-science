import concurrent.futures
import requests
import time

img_urls = [
    'https://images.unsplash.com/photo-1516117172878-fd2c41f4a759',
    'https://images.unsplash.com/photo-1532009324734-20a7a5813719',
    'https://images.unsplash.com/photo-1524429656589-6633a470097c',
    'https://images.unsplash.com/photo-1530224264768-7ff8c1789d79',
    'https://images.unsplash.com/photo-1564135624576-c5c88640f235',
    'https://images.unsplash.com/photo-1541698444083-023c97d3f4b6',
    'https://images.unsplash.com/photo-1522364723953-452d3431c267',
    'https://images.unsplash.com/photo-1513938709626-033611b8cc03',
    'https://images.unsplash.com/photo-1507143550189-fed454f93097',
    'https://images.unsplash.com/photo-1493976040374-85c8e12f0c0e',
    'https://images.unsplash.com/photo-1504198453319-5ce911bafcde',
    'https://images.unsplash.com/photo-1530122037265-a5f1f91d3b99',
    'https://images.unsplash.com/photo-1516972810927-80185027ca84',
    'https://images.unsplash.com/photo-1550439062-609e1531270e',
    'https://images.unsplash.com/photo-1549692520-acc6669e2f0c'
]

def download_img(img_url) :
    img_bytes = requests.get(img_url).content
    img_name = img_url.split('/')[3] + '.jpg'
    with open("img_folder/" + img_name, 'wb') as img_file :
        img_file.write(img_bytes)
        print(f"{img_name} has been successfully downloaded")

def main() :

    for img_url in img_urls :
        download_img(img_url)

def main_multithreading() :

    with concurrent.futures.ThreadPoolExecutor() as executor :
        executor.map(download_img, img_urls)

if __name__ == "__main__" :
    start = time.perf_counter()

    #main()
    main_multithreading()

    end = time.perf_counter()

    print(f"Total time : {end - start}")

# case1) just normal download

#photo-1516117172878-fd2c41f4a759.jpg has been successfully downloaded
#photo-1532009324734-20a7a5813719.jpg has been successfully downloaded
#photo-1524429656589-6633a470097c.jpg has been successfully downloaded
#photo-1530224264768-7ff8c1789d79.jpg has been successfully downloaded
#photo-1564135624576-c5c88640f235.jpg has been successfully downloaded
#photo-1541698444083-023c97d3f4b6.jpg has been successfully downloaded
#photo-1522364723953-452d3431c267.jpg has been successfully downloaded
#photo-1513938709626-033611b8cc03.jpg has been successfully downloaded
#photo-1507143550189-fed454f93097.jpg has been successfully downloaded
#photo-1493976040374-85c8e12f0c0e.jpg has been successfully downloaded
#photo-1504198453319-5ce911bafcde.jpg has been successfully downloaded
#photo-1530122037265-a5f1f91d3b99.jpg has been successfully downloaded
#photo-1516972810927-80185027ca84.jpg has been successfully downloaded
#photo-1550439062-609e1531270e.jpg has been successfully downloaded
#photo-1549692520-acc6669e2f0c.jpg has been successfully downloaded
#Total time : 18.974718917015707


# case2) multithread to download

#photo-1516117172878-fd2c41f4a759.jpg has been successfully downloaded
#photo-1507143550189-fed454f93097.jpg has been successfully downloaded
#photo-1564135624576-c5c88640f235.jpg has been successfully downloaded
#photo-1550439062-609e1531270e.jpg has been successfully downloaded
#photo-1504198453319-5ce911bafcde.jpg has been successfully downloaded
#photo-1549692520-acc6669e2f0c.jpg has been successfully downloaded
#photo-1516972810927-80185027ca84.jpg has been successfully downloaded
#photo-1530224264768-7ff8c1789d79.jpg has been successfully downloaded
#photo-1530122037265-a5f1f91d3b99.jpg has been successfully downloaded
#photo-1522364723953-452d3431c267.jpg has been successfully downloaded
#photo-1524429656589-6633a470097c.jpg has been successfully downloaded
#photo-1513938709626-033611b8cc03.jpg has been successfully downloaded
#photo-1532009324734-20a7a5813719.jpg has been successfully downloaded
#photo-1541698444083-023c97d3f4b6.jpg has been successfully downloaded
#photo-1493976040374-85c8e12f0c0e.jpg has been successfully downloaded
#Total time : 13.400292708014604
