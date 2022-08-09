#include "lib.hpp"
#include "lib_client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define INFO 1

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(1111);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    client cons;

    n = write(sockfd, argv[1], strlen(argv[1]));
    if (n < 0) error("ERROR writing to socket");

    bzero(buffer, 256);
        
    n = read(sockfd, buffer, 255);
    if (n < 0) error("ERROR writing to socket");
#if INFO
    cout << "get n from serv: " << buffer << "\n";
#endif
    string n_str(buffer);
    bzero(buffer, 256);
    qq n_ = str_to_qq(n_str);
    cons = client(n_, argv[1]);
    qq temp_v;

    n = write(sockfd, "y", 1);
    if (n < 0) error("ERROR writing to socket");

    n = read(sockfd, buffer, 255);
    if (n < 0) error("ERROR writing to socket");
    if (buffer[0] == 'y') {
        temp_v = cons.send_v();
#if INFO
        cout << "generated v form client: " << temp_v << "\n";
#endif
        string vvv = boost::to_string(temp_v);
        char *vv = &vvv[0];
        n = write(sockfd, vv, strlen(vv));
        if (n < 0) error("ERROR writing to socket");
    } else {
        string vvv(buffer);
#if INFO
        cout << "get v form server: " << vvv << "\n";
#endif
        qq vv = str_to_qq(vvv);
        cons.v = vv;
        n = write(sockfd, "y", 1);
        if (n < 0) error("ERROR writing to socket");
    }
    bzero(buffer, 256);
    
    n = read(sockfd, buffer, 255);
    if (n < 0) error("ERROR writing to socket");
    int check = atoi(buffer);
#if INFO
    cout << "get num of rounds from serv:  " << check << "\n";
#endif
    bzero(buffer, 256);

    //count times
    for (int i = 0; i < check; i++) {
        qq temp_x = cons.send_x();
#if INFO
        cout << "generated x form client: " << temp_x << "\n";
#endif
        string xxx = boost::to_string(temp_x);
        char *xx = &xxx[0];

        n = write(sockfd, xx, strlen(xx));
        if (n < 0) error("ERROR writing to socket");

        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR writing to socket");
        string e_str(buffer);
#if INFO
        cout << "get e from serv:  " << e_str << "\n";
#endif
        int e_ = atoi(buffer);
        bzero(buffer, 256);
        qq y = cons.send_y(e_);
#if INFO
        cout << "generated y form client: " << y << "\n";
#endif
        string yyy = boost::to_string(y);
        char *yy = &yyy[0];
        n = write(sockfd, yy, strlen(yy));
        if (n < 0) error("ERROR writing to socket");
    }
    n = read(sockfd, buffer, 255);
    if (n < 0) error("ERROR writing to socket");
    cout << "result: " << buffer << "\n";
    bzero(buffer, 256);
    
    close(sockfd);
    return 0;
}