#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
#include "user.h"
#include "vote.h"
#include "client_https.hpp"
#include <vector>
#include <map>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

typedef SimpleWeb::Client<SimpleWeb::HTTPS> HttpsClient;
using namespace rapidjson;

class ClientHandler
{
public:
    ClientHandler(QString absPath, QString username, QString pwd);
    ~ClientHandler();
    bool UpDatePostInfo(std::string prefix,std::string json_string, std::map<std::string,std::string>& m);
    bool UpDateGetInfo(int flag, std::string prefix,std::map<std::string,std::string>& m1,std::vector<std::string>& v);

    bool UpDateGetVoteListInfo(std::vector<Vote*>& v);
    bool UpDateGetVoteDetailInfo(Vote& v);

    //synthesize
    std::string Synthesize(std::string s, std::vector<std::string>&);
    std::string Synthesize(std::string key1, std::string value1);
    std::string Synthesize(std::string key1, std::string key2, std::string value1, std::string value2);
    std::string Synthesize(std::string key1, std::string key2, std::string key3, std::string value1, std::string value2,
                           std::string value3);
    std::string Synthesize(std::string key1, std::string key2, std::string mainKey, std::map<std::string,std::string>&);
    std::string Synthesize(std::vector<std::string>& usrVec,std::vector<std::string>& candVec,
                           std::string title,std::string content, std::string deadline, std::string publishDate);
    bool AddUserReq(std::string key1, std::string key2, std::string value1, std::string value2);
    bool AddUsersReq(std::string key1, std::string key2, std::string mainKey, std::map<std::string,std::string>& m);
    bool CheckAllUsersListReq(std::vector<std::string>& v);
    bool CheckActivatedUsersListReq(std::vector<std::string>& v);
    bool CheckNActivatedUsersListReq(std::vector<std::string>& v);

    bool ActivateUserReq(std::vector<std::string>&);
    bool ForbidUserReq(std::vector<std::string>&);

    bool LoginReq(std::string value1, std::string value2, std::string value3);

    //adddialog

    bool SendVoteReq();

    //publish vote
    bool SendPublishVoteReq(std::vector<std::string>& v);

    //mainwin
    bool SendCheckVotesReq(std::vector<Vote*>& v);
    bool SendCheckOneVoteReq(std::string voteName);

    void EraseUserInfo();
    std::string getUserToken() {return user->getToken();}
    int64_t getUserExpires() {return user->getExpires();}
    void setUsrInfoList(std::map<std::string,std::string> infoMap);

    //normal user
    bool ModifyPwdReq();
    //Admin read users
    bool AdminReadUsers(QString path);

    Vote* getVote() {return this->vote;}
    User* getUser() {return this->user;}
private:
    HttpsClient* client;
    static User* user;
    static Vote* vote;
    std::map<std::string,std::string> userInfoList;
};

QByteArray sha256(const std::string str);

#endif // CLIENTHANDLER_H
