#ifndef VOTE_H
#define VOTE_H
#include <iostream>
#include <vector>

class Vote
{
public:
    Vote();
    ~Vote();
    void setTitle(std::string title) {this->title=title;}
    void setContent(std::string content) {this->content=content;}
    void setUserVec(std::vector<std::string>& userVec) {this->userVec=userVec;}
    void setCandVec(std::vector<std::string>& candVec) {this->candVec=candVec;}
    void setDeadline(std::string deadline) {this->deadline=deadline;}
    void setVoteID(int voteID) {this->voteID=voteID;}
    void setFinishStatus(int finishStatus) {this->finishStatus=finishStatus;}
    void setPublishStatus(int publishStatus) {this->publishStatus=publishStatus;}
    void setPublishDate(std::string publishDate) {this->publishDate=publishDate;}

    std::string& getTitle() {return this->title;}
    std::string& getContent() {return this->content;}
    std::vector<std::string>& getUserVec() {return this->userVec;}
    std::vector<std::string>& getCandVec() {return this->candVec;}
    std::string& getDeadline() {return this->deadline;}
    int getVoteID() {return this->voteID;}
    int getFinishStatus() {return this->finishStatus;}
    int getPublishStatus() {return this->publishStatus;}
    std::string& getPublishDate() {return this->publishDate;}

private:
    std::string title;
    std::string content;
    std::vector<std::string> userVec;
    std::vector<std::string> candVec;
    std::string deadline;
    int voteID;
    int finishStatus;
    int publishStatus;
    std::string publishDate;
};

#endif // VOTE_H
