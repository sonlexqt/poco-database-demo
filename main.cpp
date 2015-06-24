#include "Poco/Data/Session.h"
//#include "Poco/Data/SQLite/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionFactory.h>
//#include <vector>
#include <iostream>


using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::MySQL::ConnectionException;
using Poco::Data::MySQL::StatementException;

struct ViewCountInfo
{
    std::string username;
    int         counter;
};

int main(int argc, char** argv)
{
    // register SQLite connector
    //Poco::Data::SQLite::Connector::registerConnector();
    
    Poco::Data::MySQL::Connector::registerConnector();

    // create a session
    //Session session("SQLite", "sample.db");
    Session session("MySQL", "host=127.0.0.1;port=3306;db=view_count_vng;user=view_count_user;password=secret;compress=true;auto-reconnect=true");

    // drop sample table, if it exists
    session << "DROP TABLE IF EXISTS view_count_info", now;

    // (re)create table
    session << "CREATE TABLE view_count_info (username VARCHAR(30) NOT NULL, counter INTEGER(5) NOT NULL)", now;

    // insert some rows
    ViewCountInfo vcInfo = 
    {
        "SonLe",
        7
    };

    Statement insert(session);
    insert << "INSERT INTO view_count_info VALUES(?, ?)",
        use(vcInfo.username),
        use(vcInfo.counter);
    insert.execute();

    vcInfo.username    = "LanLe";
    vcInfo.counter = 25;
    insert.execute();

    Statement myUpdate(session);
    std::string updateStmt = "UPDATE view_count_info SET counter = " 
            + std::string("69") 
            + " WHERE username = '" + vcInfo.username + "'";
    myUpdate << updateStmt;
    myUpdate.execute();
    
//    Statement myUpdate(session);
//    myUpdate << "UPDATE view_count_info SET counter = 100 WHERE username = 'LanLe'";
//    myUpdate.execute();      
    
    // a simple query
    Statement select(session);
    select << "SELECT username, counter FROM view_count_info",
        into(vcInfo.username),
        into(vcInfo.counter),
        range(0, 1); //  iterate over result set one row at a time

    while (!select.done())
    {
        select.execute();
        std::cout << vcInfo.username << " " << vcInfo.counter << " " << std::endl;
    }

    return 0;
}
