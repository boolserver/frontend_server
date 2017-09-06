.PHONY: all requirements

all:
	rm -f clients_frontend.out backend_frontend.out
	gcc main_frontend_clients.c serverside_for_clients.c client_for_msg_queue.c -o clients_frontend.out -lrt -luuid
	gcc server_for_backend.c serverside_for_clients.c client_for_msg_queue.c -o backend_frontend.out -lrt -luuid

requirement:
	sudo apt-get install uuid-dev

run_client_frontend:
	./clients_frontend.out

run_backend_frontend:
	./backend_frontend.out
