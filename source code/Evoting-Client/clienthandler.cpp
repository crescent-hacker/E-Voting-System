#include "clienthandler.h"
#include "client_https.hpp"
#include "mainwin.h"
#include "modifypwddialog.h"
#include "usermainwindow.h"
#include "readconf.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QException>
#include <QFile>
#include <sstream>
#include <QDebug>
#include <map>
#include <stdlib.h>


using namespace std;
using namespace rapidjson;
using namespace boost::property_tree;

QByteArray sha256(const string str)
{
    QByteArray input(str.c_str());
    QByteArray output=QCryptographicHash::hash(input,QCryptographicHash::Sha256);
    return output.toBase64();
}

User* ClientHandler::user=new User("");

Vote* ClientHandler::vote=new Vote();

ClientHandler::ClientHandler(QString path, QString username, QString pwd)
{
    QString certPath=path+QString("/client.crt");
    QString keyPath=path+QString("/client.key");
    QString verifyPath=path+QString("/ca.cer");
//    HttpsClient client("csci969server:8080", true, certPath.toStdString(), keyPath.toStdString(), verifyPath.toStdString());
    this->client=new HttpsClient("localhost:8080", true, certPath.toStdString(), keyPath.toStdString(), verifyPath.toStdString());
//    this->client=new HttpsClient("10.12.14.2:8080", true, certPath.toStdString(), keyPath.toStdString(), verifyPath.toStdString());

    //request
    map<string,string> m;
    m["Content-Type"]="application/json";
    m["connection"]="keep-alive";
    try{
        //1.receive [t_token+t]
        auto r=client->request("GET", "/pre_login","",m);
        ostringstream os1;
        os1 << r->content.rdbuf();
        std::cout << os1.str() << std::endl;
        Document document1;
        document1.Parse(os1.str().c_str());
        int64_t t=document1["t"].GetInt64();
        std::string tStr=boost::lexical_cast<std::string>(t);
        getUser()->setExpires(t);

        std::cout << tStr << std::endl;
        std::string t_token=document1["t_token"].GetString();
        getUser()->setT_token(t_token);

        std::string input=username.toStdString()+pwd.toStdString()+
                std::string("3qaz2wsx3edc4rfv!@#$%^&qqtrtRTWDFHAJBFHAGFUAHKJFHAJHFJHAJWRFA");
        std::cout << "input: " << input << std::endl;
        QByteArray encryptInput=sha256(input);
        std::string eStr1=encryptInput.toStdString();
        std::cout << eStr1 << std::endl;

        std::string InputAll=eStr1+tStr;
        std::cout << InputAll << std::endl;
        QByteArray encryptAll=sha256(InputAll);
        std::string eStr2=encryptAll.toStdString();
        std::cout << "length: " << eStr2.length() << std::endl;
        std::cout << "encryptAll: " << eStr2 << std::endl;

        if(LoginReq(t_token,eStr2,username.toStdString())==false)
            std::cout << "login fail" << std::endl;

    }catch(QException &e){
        qDebug() << e.what();
    }

}

ClientHandler::~ClientHandler()
{
    delete client;
}

bool ClientHandler::UpDatePostInfo(std::string prefix,
                                std::string json_string, std::map<std::string,std::string>& m){
    auto r1=client->request("POST", prefix.c_str(), json_string, m);
    Document document;
    ostringstream os;
    os << r1->content.rdbuf();
    document.Parse(os.str().c_str());
    std::cout << os.str() << std::endl;
    if(document["success"].GetBool()==false){
        std::cout << "post fail" << std::endl;
        return false;
    }
    else{
    try{
        if(document.HasMember("token")){
            std::cout << "hash token member" << std::endl;
            user->setToken(document["token"].GetString());
            user->setExpires(document["expires"].GetInt64());
            user->setIsAdmin(document["is_admin"].GetBool());
            //admin login
            if(user->getIsAdmin()==true){
                //add users from csv file
                if(AdminReadUsers("/Users/ios/Desktop/data.csv")==false){
                    QMessageBox* box=new QMessageBox(QMessageBox::Warning,"read users","users have been read from csv file beforehand");
                    box->show();
                }
                else{
                    QMessageBox* box=new QMessageBox(QMessageBox::Information,"read users","users have been read successfully");
                    box->show();
                }
                QMainWindow* window=new MainWin();
                window->show();
            }
            //normal user
            else{
                //user login
                if(document.HasMember("is_pwd_modified")){
                    user->setPwdModified(document["is_pwd_modified"].GetInt());
                }
                std::cout << "modified :" << user->getPwdModified() << std::endl;
                if(getUser()->getPwdModified()==0){
                    ModifyPwdDialog* window=new ModifyPwdDialog();
                    window->show();
                }else if(getUser()->getPwdModified()==1){
                    UserMainWindow* window=new UserMainWindow();
                    window->show();
                }
            }

          }
        }catch(QException &e){
            qDebug() << e.what();
        }
        return true;
    }
}

bool ClientHandler::UpDateGetInfo(int flag, std::string prefix,std::map<std::string,std::string>& m1, std::vector<std::string>& v){
    auto r1=client->request("GET", prefix.c_str(),"",m1);
    Document document,subDocument;
    ostringstream os;
    os << r1->content.rdbuf();
    document.Parse(os.str().c_str());

    if(document["success"].GetBool()==false)
        return false;
    else{
        if(document.HasMember("user_list")){
              const Value& arr=document["user_list"];

              for (SizeType i = 0; i < arr.Size(); i++){
              //All users
              if(flag==2){
                  Value::ConstMemberIterator itr =arr[i].GetObject().FindMember("username");
                  if(itr!=arr[i].GetObject().MemberEnd()){
                      v.push_back(itr->value.GetString());
        //                      std::cout << itr->value.GetString() << std::endl;
                  }
              }
              //no activated users
              else if(flag==0){
                  Value::ConstMemberIterator itr1 =arr[i].GetObject().FindMember("is_activated");
                  if(itr1!=arr[i].GetObject().MemberEnd()){
                      if(itr1->value.GetInt()==0){
                          Value::ConstMemberIterator itr =arr[i].GetObject().FindMember("username");
                          if(itr!=arr[i].GetObject().MemberEnd()){
                              v.push_back(itr->value.GetString());
        //                              std::cout << "0:" <<itr->value.GetString() << std::endl;

                          }
                      }
                  }
              }
              //activated users
              else if(flag==1){
                  Value::ConstMemberIterator itr1 =arr[i].GetObject().FindMember("is_activated");
                  if(itr1!=arr[i].GetObject().MemberEnd()){
                      if(itr1->value.GetInt()==1){
                          Value::ConstMemberIterator itr =arr[i].GetObject().FindMember("username");
                          if(itr!=arr[i].GetObject().MemberEnd()){
                              v.push_back(itr->value.GetString());
        //                              std::cout << "1:" << itr->value.GetString() << std::endl;

                          }
                      }
                  }
              }
            }
        }
        return true;
    }
}

bool ClientHandler::UpDateGetVoteListInfo(std::vector<Vote*>& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
    std::cout << "token is: " << getUserToken() << std::endl;

    try{
    auto r1=client->request("GET", "/get_vote_list_m","", m1);
//    Document document;
//    ostringstream os;
//    os << r1->content.rdbuf();
//    document.Parse(os.str().c_str());
//    std::cout << os.str() << std::endl;

//    if(document["success"].GetBool()==false)
//        return false;
//    else{
//        //parse document
//        if(document.HasMember("vote_list")){
//            const Value& arr=document["vote_list"];
//            for (SizeType i = 0; i < arr.Size(); i++){
//                Vote* vote=new Vote();
//                Value::ConstMemberIterator itr1;
//                itr1 =arr[i].GetObject().FindMember("vote_id");
//                if(itr1!=arr[i].GetObject().MemberEnd()){
//                    vote->setVoteID(itr1->value.GetInt());
//                }
//                itr1=arr[i].GetObject().FindMember("vote_title");
//                if(itr1!=arr[i].GetObject().MemberEnd()){
//                    vote->setTitle(itr1->value.GetString());
//                }
//                itr1=arr[i].GetObject().FindMember("vote_content");
//                if(itr1!=arr[i].GetObject().MemberEnd()){
//                    vote->setContent(itr1->value.GetString());
//                }
//                itr1=arr[i].GetObject().FindMember("is_published");
//                if(itr1!=arr[i].GetObject().MemberEnd()){
//                    vote->setPublishStatus(itr1->value.GetInt());
//                }
//                itr1=arr[i].GetObject().FindMember("deadline_date");
//                if(itr1!=arr[i].GetObject().MemberEnd()){
//                    vote->setDeadline(itr1->value.GetString());
//                }
//                itr1=arr[i].GetObject().FindMember("is_close");
//                if(itr1!=arr[i].GetObject().MemberEnd()){
//                    vote->setFinishStatus(itr1->value.GetInt());
//                }
//                v.push_back(vote);
//            }
//        }
//        return true;
//    }
    }catch(QException &e){
        qDebug() << e.what();
    }
    return true;
}

bool ClientHandler::UpDateGetVoteDetailInfo(Vote& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();

    auto r1=client->request("GET", "/get_vote_detail","", m1);
    Document document;
    ostringstream os;
    os << r1->content.rdbuf();
    document.Parse(os.str().c_str());
    std::cout << os.str() << std::endl;

    if(document["success"].GetBool()==false)
        return false;
    else{
        const Value& vID=document["vote_id"];
        v.setVoteID(vID.GetInt());
        const Value& vTitle=document["vote_title"];
        v.setTitle(vTitle.GetString());
        const Value& vContent=document["vote_content"];
        v.setContent(vContent.GetString());
        const Value& vPublishStatus=document["is_published"];
        v.setPublishStatus(vPublishStatus.GetInt());
        const Value& vDeadLine=document["deadline_date"];
        v.setDeadline(vDeadLine.GetString());
        const Value& vFinishedStatus=document["is_close"];
        v.setFinishStatus(vFinishedStatus.GetInt());

        const Value& arrUser=document["user_list"];
        Value::ConstMemberIterator itr1;
        std::vector<std::string> userVec;
        for (SizeType i = 0; i < arrUser.Size(); i++){
            itr1=arrUser[i].GetObject().FindMember("username");
            userVec.push_back(itr1->value.GetString());
        }
        v.setUserVec(userVec);

        const Value& arrCand=document["cand_list"];
        Value::ConstMemberIterator itr2;
        std::vector<std::string> candVec;
        for (SizeType i = 0; i < arrCand.Size(); i++){
            itr2=arrCand[i].GetObject().FindMember("username");
            candVec.push_back(itr2->value.GetString());
        }
        v.setCandVec(candVec);
        return true;
    }
}

string ClientHandler::Synthesize(string s, vector<string>& v)
{
    StringBuffer sb;
    Document d;
    Document::AllocatorType& allocator = d.GetAllocator();

    Value username(s.c_str(),s.length(),allocator);
    Value arr(kArrayType);
    std::vector<std::string>::iterator it=v.begin();
    for(;it!=v.end();++it){
        Value name(it->c_str(),it->length(),allocator);
        arr.PushBack(name,allocator);
    }
    Value all(kObjectType);
    all.AddMember(username,arr,allocator);
    PrettyWriter<StringBuffer> writer(sb);
    all.Accept(writer);
    return sb.GetString();
}

std::string ClientHandler::Synthesize(std::string key1, std::string key2, std::string value1, std::string value2)
{
    StringBuffer sb;
    Document d;
    Document::AllocatorType& allocator = d.GetAllocator();
    Value o(kObjectType);
    Value v1(value1.c_str(),value1.length(),allocator);
    Value v2(value2.c_str(),value2.length(),allocator);
    o.AddMember(StringRef(key1.c_str()),v1,allocator);
    o.AddMember(StringRef(key2.c_str()),v2,allocator);
    PrettyWriter<StringBuffer> writer(sb);
    o.Accept(writer);
    return sb.GetString();
}

std::string ClientHandler::Synthesize(std::string key1, std::string key2, std::string mainKey, std::map<std::string,std::string>& m)
{
    StringBuffer sb;
    Document d;
    Document::AllocatorType& allocator = d.GetAllocator();

    Value user_list(mainKey.c_str(),mainKey.length(),allocator);
    Value arr(kArrayType);
    std::map<std::string,std::string>::iterator it=m.begin();
    for(;it!=m.end();++it){
        Value o(kObjectType);
        Value name(it->first.c_str(),it->first.length(),allocator);
        Value email(it->second.c_str(),it->second.length(),allocator);
        o.AddMember(StringRef(key1.c_str()), name, d.GetAllocator());
        o.AddMember(StringRef(key2.c_str()), email, d.GetAllocator());
        arr.PushBack(o,allocator);
    }

    Value all(kObjectType);
    all.AddMember(user_list,arr,allocator);
    PrettyWriter<StringBuffer> writer(sb);
    all.Accept(writer);
    return sb.GetString();
}

std::string ClientHandler::Synthesize(std::string key1, std::string value1)
{
    StringBuffer sb;
    Document d;
    Document::AllocatorType& allocator = d.GetAllocator();
    Value o(kObjectType);
    Value v1(value1.c_str(),value1.length(),allocator);
    o.AddMember(StringRef(key1.c_str()),v1,allocator);
    PrettyWriter<StringBuffer> writer(sb);
    o.Accept(writer);
    std::cout << sb.GetString() << std::endl;
    return sb.GetString();
}

std::string ClientHandler::Synthesize(std::vector<std::string>& usrVec,std::vector<std::string>& candVec,
                       std::string title,std::string content,std::string deadline, std::string publishDate)
{
    StringBuffer sb;
    Document d;
    Document::AllocatorType& allocator = d.GetAllocator();
    //add candidate array
    Value o(kObjectType);
    Value vTitle(title.c_str(),title.length(),allocator);
    o.AddMember(StringRef("title"),vTitle,allocator);
    Value arrCand(kArrayType);
    std::vector<std::string>::iterator it=candVec.begin();
    for(;it!=candVec.end();++it){
        Value ob(kObjectType);
        Value candidate(it->c_str(),it->length(),allocator);
        ob.AddMember(StringRef("candname"),candidate,allocator);
        arrCand.PushBack(ob,allocator);
    }
    o.AddMember(StringRef("candidates"),arrCand,allocator);
    //add eligible voter
    Value arrVoter(kArrayType);
    for(it=usrVec.begin();it!=usrVec.end();++it){
        Value ob(kObjectType);
        Value user(it->c_str(),it->length(),allocator);
        ob.AddMember(StringRef("username"),user,allocator);
        arrVoter.PushBack(ob,allocator);
    }
    o.AddMember(StringRef("eligible_voters"),arrVoter,allocator);
    Value vContent(content.c_str(),content.length(),allocator);
    o.AddMember(StringRef("content"),vContent,allocator);
    Value vDeadline(deadline.c_str(),deadline.length(),allocator);
    o.AddMember(StringRef("deadline"),vDeadline,allocator);
    Value vPushlishDate(publishDate.c_str(),deadline.length(),allocator);
    o.AddMember(StringRef("publishDate"),vPushlishDate,allocator);

    PrettyWriter<StringBuffer> writer(sb);
    o.Accept(writer);
    std::cout << sb.GetString() << std::endl;
    return sb.GetString();
}

std::string ClientHandler::Synthesize(std::string key1, std::string key2, std::string key3, std::string value1, std::string value2,
                       std::string value3)
{
    StringBuffer sb;
    Document d;
    Document::AllocatorType& allocator = d.GetAllocator();
    Value o(kObjectType);
    Value v1(value1.c_str(),value1.length(),allocator);
    Value v2(value2.c_str(),value2.length(),allocator);
    Value v3(value3.c_str(),value3.length(),allocator);
    o.AddMember(StringRef(key1.c_str()),v1,allocator);
    o.AddMember(StringRef(key2.c_str()),v2,allocator);
    o.AddMember(StringRef(key3.c_str()),v3,allocator);
    PrettyWriter<StringBuffer> writer(sb);
    o.Accept(writer);
    return sb.GetString();
}

bool ClientHandler::AddUsersReq(std::string key1, std::string key2, std::string mainKey, std::map<std::string,std::string>& m)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();

    std::string json_string=Synthesize(key1, key2, mainKey, m);
    std::cout << json_string << std::endl;
    if(UpDatePostInfo("/add_users", json_string, m1)==true)
        return true;
    else
        return false;
}

bool ClientHandler::AddUserReq(std::string key1, std::string key2, std::string value1, std::string value2)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();

    std::string json_string=Synthesize(key1, key2, value1, value2);
    std::cout << json_string << std::endl;
    if(UpDatePostInfo("/add_user", json_string, m1)==true)
        return true;
    else
        return false;
}

bool ClientHandler::CheckAllUsersListReq(std::vector<std::string>& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
    bool ret=UpDateGetInfo(2,"/get_user_list", m1, v);
    if(ret==true)
        return true;
    else
        return false;
}

bool ClientHandler::CheckActivatedUsersListReq(std::vector<std::string>& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
    bool ret=UpDateGetInfo(1,"/get_user_list", m1, v);
    if(ret==true)
        return true;
    else
        return false;

}

bool ClientHandler::CheckNActivatedUsersListReq(std::vector<std::string>& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
    bool ret=UpDateGetInfo(0,"/get_user_list", m1, v);
    if(ret==true)
        return true;
    else
        return false;

}

bool ClientHandler::ActivateUserReq(std::vector<std::string>& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
    std::string json_string=Synthesize("username",v[0]);
    std::cout << json_string << std::endl;
    if(UpDatePostInfo("/activate_user",json_string,m1)==true)
        return true;
    else
        return false;
}

bool ClientHandler::ForbidUserReq(std::vector<std::string>& v)
{
        std::map<std::string,std::string> m1;
        m1["Content-Type"]="application/json";
        m1["connection"]="keep-alive";
        m1["x-access-token"]=getUserToken();
        std::string json_string=Synthesize("username",v[0]);
        if(UpDatePostInfo("/disable_user",json_string,m1)==true)
            return true;
        else
            return false;
}

bool ClientHandler::SendVoteReq()
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
    std::string json_string=Synthesize(vote->getUserVec(),vote->getCandVec(),
                              vote->getTitle(),vote->getContent(),vote->getDeadline(),vote->getPublishDate());
//    std::cout << json_string << std::endl;
    if(UpDatePostInfo("/propose_vote",json_string,m1)==true)
        return true;
    else
        return false;
}

void ClientHandler::EraseUserInfo(){
    user->setToken("");
    user->setExpires(0);
}

void ClientHandler::setUsrInfoList(std::map<std::string,std::string> infoMap)
{
    std::map<std::string,std::string>::iterator it=infoMap.begin();
    for(;it!=infoMap.end();++it)
    {
        this->userInfoList.insert(std::pair<std::string,std::string>(it->first,it->second));
    }
}

bool ClientHandler::LoginReq(std::string value1, std::string value2, std::string value3)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    std::string json_string=Synthesize("t_token","password","username",value1,value2,value3);
    std::cout << json_string << std::endl;
    if(UpDatePostInfo("/login",json_string,m1)==true)
        return true;
    else
        return false;
}

bool ClientHandler::SendCheckVotesReq(std::vector<Vote*>& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
    if(UpDateGetVoteListInfo(v)==false)
        return false;
    else
        return true;
}

bool ClientHandler::SendCheckOneVoteReq(std::string voteName)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();

    std::string json_string=Synthesize("voteName",voteName);

    if(UpDatePostInfo("/",json_string,m1)==false)
        return false;
    else
        return true;
}

bool ClientHandler::SendPublishVoteReq(std::vector<std::string>& v)
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();

    std::string json_string=Synthesize("publish",v);

    if(UpDatePostInfo("/",json_string,m1)==false)
        return false;
    else
        return true;
}

bool ClientHandler::ModifyPwdReq()
{
    std::map<std::string,std::string> m1;
    m1["Content-Type"]="application/json";
    m1["connection"]="keep-alive";
    m1["x-access-token"]=getUserToken();
//    std::cout << "token is :" << getUserToken() << std::endl;
//    std::cout << "pwd is" << getUser()->getPwd() << std::endl;
    std::string json_string=Synthesize("username", "password", getUser()->getUserName(),
                                       getUser()->getPwd());

    if(UpDatePostInfo("/modify_pwd",json_string,m1)==false)
        return false;
    else
        return true;
}

bool ClientHandler::AdminReadUsers(QString path)
{
    QFile* file=new QFile(path);
//    QFile* file=new QFile("/Users/ios/Desktop/data.csv");
    ReadConf readConf(file);
    std::map<std::string,std::string> m;
    readConf.matchCvs(m);
    std::map<std::string,std::string>::iterator it=m.begin();
    for(;it!=m.end();++it){
        std::cout << it->first << " " << it->second << std::endl;
    }
    if(AddUsersReq("username", "email", "user_list", m)==false)
        return false;
    else
        return true;
}
