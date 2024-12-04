#include <memory>
#include <pjsua2.hpp>
#include <iostream>
#include "inherited_classes.h"
#include "auxiliary_functions.h"
#include <fstream>

int main(int argc, char** argv)
{
    int transportConfPort = 5060;
    std::string Uri = "sip:192.168.0.15";
    std::string regUri = "sip:sip.pjsip.org";
    std::string scheme = "digest";
    std::string realm = "*";
    std::string username = "test";
    std::string data = "passwd";

    std::fstream confFile;
    confFile.open("account_conf.txt", std::ios::in);

    if(confFile.is_open())
    {
        /*reading configs from the file*/
        /*make this code safer*/
        /*make it more beautiful*/
        std::string tmp;
        int pos = 0;
        
        /*transport port*/
        try 
        {
            readConfLine(confFile, transportConfPort);
        }
        catch(const std::exception& e)
        {
            std::cerr << "account_conf.txt: " << e.what() << " in line 1\n";
            confFile.close();
            return 1;
        }

        /*uri*/
        try 
        {
            readConfLine(confFile, Uri);
        }
        catch(const std::exception& e)
        {
            std::cerr << "account_conf.txt: " << e.what() << " in line 2\n";
            confFile.close();
            return 1;
        }

        /*reg uri*/
        try 
        {
            readConfLine(confFile, regUri);
        }
        catch(const std::exception& e)
        {
            std::cerr << "account_conf.txt: " << e.what() << " in line 3\n";
            confFile.close();
            return 1;
        }

        /*scheme*/
        try 
        {
            readConfLine(confFile, transportConfPort);
        }
        catch(const std::exception& e)
        {
            std::cerr << "account_conf.txt: " << e.what() << " in line 4\n";
            confFile.close();
            return 1;
        }

        /*realm*/
        try 
        {
            readConfLine(confFile, realm);
        }
        catch(const std::exception& e)
        {
            std::cerr << "account_conf.txt: " << e.what() << " in line 5\n";
            confFile.close();
            return 1;
        }

        /*username*/
        try 
        {
            readConfLine(confFile, username);
        }
        catch(const std::exception& e)
        {
            std::cerr << "account_conf.txt: " << e.what() << " in line 6\n";
            confFile.close();
            return 1;
        }

        /*data*/
        try 
        {
            readConfLine(confFile, data);
        }
        catch(const std::exception& e)
        {
            std::cerr << "account_conf.txt: " << e.what() << " in line 7\n";
            confFile.close();
            return 1;
        }

        confFile.close();

        std::cout << "Configuration has been read" << std::endl;
    }
    else
    {
        /*config file doesn't exist. Creating a new one...*/
        std::cout << "Couldn't find account_conf.txt file. Creating a new one with default configs" << std::endl;
        confFile.open("account_conf.txt", std::ios::out | std::ios::trunc);
        if(!confFile.is_open())
            throw std::runtime_error{"Cannot create a new account_conf.txt file\n"};
        
        confFile << "transport_port=" << transportConfPort << std::endl;
        confFile << "URI=" << Uri << std::endl;
        confFile << "regURI=" << regUri << std::endl;
        confFile << "scheme=" << scheme << std::endl;
        confFile << "realm=" << realm << std::endl;
        confFile << "username=" << username << std::endl;
        confFile << "password=" << data << std::endl;

        confFile.close();
    }

    pj::Endpoint ep;
    ep.libCreate();

    //Initialize endpoint
    pj::EpConfig epConf;
    ep.libInit(epConf);

    //Create SIP transport
    pj::TransportConfig transportConf;
    transportConf.port = transportConfPort; 

    try
    {
        ep.transportCreate(PJSIP_TRANSPORT_UDP, transportConf);
    }
    catch(pj::Error& e)
    {
        std::cerr << e.info() << '\n';
        return 1;
    }
    
    /*Start the library*/
    ep.libStart();
    std::cout << "Program started" << std::endl;

    /*Configure an account*/
    pj::AccountConfig accConf;
    accConf.idUri = Uri;
    accConf.regConfig.registrarUri = regUri;
    accConf.sipConfig.authCreds.push_back(pj::AuthCredInfo(scheme, realm, username, 0, data));

    /*Create the account*/
    std::unique_ptr<MyAccount> account(new MyAccount);
    try
    {
        account->create(accConf);
        std::cout << "Account created" << std::endl;
    }
    catch(pj::Error& e)
    {
        std::cerr << "Couldn't create an account " << e.info() << std::endl;
        return 1;
    }

    while(true)
    {
        /*endless loop for waiting for calls*/
    }

    return 0;
}
