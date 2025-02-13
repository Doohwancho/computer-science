---\
How to run?

```
gcc http_server.c -o http_server
./http_server
```

```
# Test root path (200 OK)
curl -v http://localhost:8080/

# Test non-existent path (404)
curl -v http://localhost:8080/notfound

# Test different method (500)
curl -v -X POST http://localhost:8080/
```

---\
What 


http request parser(GET request) 추가 
