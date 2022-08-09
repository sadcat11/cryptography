#include "lib.hpp"
#include "lib_server.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define INFO 1

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(1111);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
             error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    int e;
    unique_ptr<server> serv(new server());
    const qq n_serv = serv->send_n();
    ull res_check = 0;
    int check = (argc > 1) ? atoi(argv[1]) : 10;
    string check_arg = std::to_string(check);
    char *arg_check = &check_arg[0];

    string sss = boost::to_string(n_serv);
    char *ss = &sss[0];

    bzero(buffer, 256);
    while(1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        n = read(newsockfd, buffer, 255);
        if (n < 0) error("ERROR writing to socket");
        string name(buffer);
        cout << "name: " << name << "\n";

        bzero(buffer, 256);
        n = write(newsockfd, ss, strlen(ss));       //send n
        if (n < 0) error("ERROR writing to socket");

        n = read(newsockfd, buffer, 255);       //check
        if (n < 0) error("ERROR writing to socket");
        bzero(buffer, 256);

        ifstream file;
        file.open("server/" + name + "_v.txt");
        if (!file) {
            n = write(newsockfd, "y", 1);
            if (n < 0) error("ERROR writing to socket");
        
            n = read(newsockfd, buffer, 255);           //get v
            if (n < 0) error("ERROR writing to socket");

            string v_str(buffer);
            bzero(buffer, 256);
#if INFO
            cout << "get v from client: " << v_str << "\n";
#endif
            qq v = str_to_qq(v_str);
            serv->v = v;
            file.close();
            ofstream file1("server/" + name + "_v.txt");
            file1 << v << "\n";
            file1.close();
        } else {
            string strv;
            if (file.is_open())
                getline(file, strv);
            serv->v = str_to_qq(strv);
            char *st = &strv[0];
            n = write(newsockfd, st, strlen(st));
            if (n < 0) error("ERROR writing to socket");
            file.close();
            n = read(newsockfd, buffer, 255);       //check
            if (n < 0) error("ERROR writing to socket");
            bzero(buffer, 256);
        }
        usleep(300);
        n = write(newsockfd, arg_check, strlen(arg_check));
        cout << arg_check << "\n";
        if (n < 0) error("ERROR writing to socket");
        //count times
        for (int i = 0; i < check; i++) {
            n = read(newsockfd, buffer, 255);           //get x
            if (n < 0) error("ERROR writing to socket");
#if INFO
            cout << "get x from client: " << buffer << "\n";
#endif
            string x_str(buffer);
            bzero(buffer, 256);
            qq x = str_to_qq(x_str);
            e = serv->create_bit_e(x);
#if INFO
            cout << "generated e form server: " << e << "\n";
#endif
            string eee = std::to_string(e);
            char *ee = &eee[0];
            n = write(newsockfd, ee, strlen(ee));       //send e
            if (n < 0) error("ERROR writing to socket");

            n = read(newsockfd, buffer, 255);           //get y
            if (n < 0) error("ERROR writing to socket");
#if INFO
            cout << "get y from client: " << buffer << "\n"; 
#endif
            string y_str(buffer);
            bzero(buffer, 256);
            qq y = str_to_qq(y_str);

            if (serv->check_proof(y, x)) {
#if INFO
                cout << "ok\n";
#endif
            } else {
#if INFO
                cout << "not ok\n";
#endif
                res_check++;
            }
        }
        if (res_check == 0) {
            cout << "===============\n" + name + ": pass\n===============\n";
            n = write(newsockfd, "pass", 4);
            if (n < 0) error("ERROR writing to socket");
        } else {
            cout << "===============\n" + name + ": passn't\n===============\n";
            n = write(newsockfd, "passn't", 7);
            if (n < 0) error("ERROR writing to socket");
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0; 
}