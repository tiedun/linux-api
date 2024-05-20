CC:=gcc

tcp_server: tcp_server.c
	-$(CC) -o $@ $^

tcp_client: tcp_client.c
	$(CC) -o $@ $^

daemon_test: daemon_test.c tcp_server tcp_client
	-$(CC) -o $@ $<
	-./$@
	-rm ./$@ ./$(word 2, $^) ./$(word 3, $^)