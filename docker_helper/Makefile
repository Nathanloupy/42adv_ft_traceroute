run: build
	docker run -d --name ft_traceroute -v $(shell pwd)/../:/app ft_traceroute

build:
	docker build -t ft_traceroute .

exec:
	docker exec -it ft_traceroute bash

stop:
	docker stop ft_traceroute

fclean: stop
	docker rm ft_traceroute
	docker image rm ft_traceroute
 
re: run build exec stop fclean
