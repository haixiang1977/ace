g++ -g logging_server.cpp logging_handler.cpp iterative_logging_server.cpp -lACE -o iterative_logging_server.out -I/usr/local/include/ -L/usr/local/lib
g++ -g logging_client.cpp -lACE -o logging_client.out -I/usr/local/include/ -L/usr/local/lib

