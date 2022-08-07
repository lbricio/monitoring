## Monitoring

<img width="300" alt="image" src="https://user-images.githubusercontent.com/81334995/182636898-62326309-62e7-43eb-ba52-03210c27b59d.png">

### what is monitoring?

a simple application to monitoring any services specified by user, can be either local host or web. monitoring continuously send HTTP/PING/DNS requests to address specifies in config file `monitoring.db`, the HTTP and DNS requests are made using cURL C API, and PING request are made by using local default PING bin.

`monitoring.db` should follow a specific set of ruls defined below in docummentation.

### execute monitoring:
```
 ./monitoring
```
### setting monitoring.db
(fields **must** be separated by **one** tab)
| PROTOCOL      | EXPECTED FIELDS        |
| ------------- |:-------------|
| HTTP | name, HTTP, address, method, expected code, timeout |
| PING | name, PING, address, timeout |
| DNS | name, PING, address, timeout, DNS Address |

### example monitoring.db
```
intra	      HTTP	intra.42.fr	        GET	200	120
game	      PING	game.42sp.org.br	60
workspaces    PING	workspaces.42sp.org.br	60
youtube	      DNS	https://youtube.com	200	8.8.8.8
```
