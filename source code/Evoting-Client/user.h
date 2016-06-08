#ifndef USER_H
#define USER_H
#include <string>
#include <QString>

class User
{
public:
    User(std::string userName);
    std::string getToken() {return token;}
    int64_t getExpires() {return expires;}
    int getPwdModified() {return this->pwdModified;}
    std::string getUserName() {return this->userName;}
    std::string getPwd() {return this->pwd;}
    std::string getT_token() {return this->t_token;}
    bool getIsAdmin() {return this->isAdmin;}


    void setToken(std::string str) {token=str;}
    void setExpires(int64_t val) {expires=val;}
    void setUserName(std::string userName) {this->userName=userName;}
    void setPwdModified(int i) {this->pwdModified=i;}
    void setPwd(std::string pwd) {this->pwd=pwd;}
    void setT_token(std::string t_token) {this->t_token=t_token;}
    void setIsAdmin(bool isAdmin) {this->isAdmin=isAdmin;}

private:
    std::string token;
    int64_t expires;
    std::string pwd;
    std::string userName;
    int pwdModified;
    std::string t_token;
    bool isAdmin;
};

#endif // USER_H
