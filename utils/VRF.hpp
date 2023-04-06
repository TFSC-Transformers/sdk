#ifndef _VRF_H_
#define _VRF_H_

#include "EDManager.h"

class VRF
{
    public:
        VRF() = default;
        ~VRF() = default;
        
        

        int CreateVRF(EVP_PKEY* pkey, const std::string& input, std::string & output, std::string & proof)
        {
            std::string hash = getsha256hash(input);
	        if(ED25519SignMessage(hash, pkey, proof) == false)
            {
                return -1;
            }

            output = getsha256hash(proof);
            return 0;
        }

        int VerifyVRF(EVP_PKEY* pkey, const std::string& input, std::string & output, std::string & proof)
        {
            std::string hash = getsha256hash(input);
            if(ED25519VerifyMessage(hash, pkey, proof) == false)
            {
                return -1;
            }

            output = getsha256hash(proof);
            return 0;
        }

        int testVRF()
        {
            ED ed;
            auto pkey=ed.pkey;

            std::string test="hello,world!";
            std::string output,proof;
            int ret = CreateVRF(pkey, test, output, proof);
            if(ret != 0){
                std::cout << "error create:" << ret << std::endl;
                return -2;
            }

            ret = VerifyVRF(pkey, test, output, proof);
            if(ret != 0){
                std::cout << "error verify: " << ret << std::endl;
                return -1;
            }

            return 0;
            
        }
        
        int GetRandNum(std::string data, uint32_t limit)
        {
            auto value = stringToll(data);
            return  value % limit;
        }

        double GetRandNum(const std::string& data)
        {
            auto value = stringToll(data);
            return  double(value % 100) / 100.0;
        }

        void addVrfInfo(const std::string & TxHash,Vrf & info){
            std::unique_lock<std::shared_mutex> lck(vrfInfoMutex);
            auto ite=vrfCache.find(TxHash);
            if(ite==vrfCache.end()){
                vrfCache[TxHash]=info;
            }
        }

        void removeVrfInfo(const std::string & TxHash)
        {
            std::unique_lock<std::shared_mutex> lck(vrfInfoMutex);
            auto ite = vrfCache.find(TxHash);
            if(ite != vrfCache.end()){
                vrfCache.erase(ite);
            }
        }

        bool  getVrfInfo(const std::string & TxHash,std::pair<std::string,Vrf> & vrf){
            std::shared_lock<std::shared_mutex> lck(vrfInfoMutex);
            auto ite= vrfCache.find(TxHash);
            if(ite!=vrfCache.end()){
                vrf.first=ite->first;
                vrf.second=ite->second;
                return true;
            }
            return false;
        }

         void addVerifyNodes(const std::string & TxHash,std::vector<std::string> & base58_s){
            std::unique_lock<std::shared_mutex> lck(vrfNodeMutex);
            auto ite=vrfVerifyNode.find(TxHash);
            if(ite==vrfVerifyNode.end()){
                vrfVerifyNode[TxHash]=base58_s;
            }
        }

        void removeVerifyNodes(const std::string & TxHash)
        {
            std::unique_lock<std::shared_mutex> lck(vrfNodeMutex);
            auto ite = vrfVerifyNode.find(TxHash);
            if(ite != vrfVerifyNode.end())
            {
                vrfVerifyNode.erase(ite);
            }
        }
        
        bool  getVerifyNodes(const std::string & TxHash,std::pair<std::string,std::vector<std::string>> & res){
            std::shared_lock<std::shared_mutex> lck(vrfNodeMutex);
            auto ite= vrfVerifyNode.find(TxHash);
            if(ite!=vrfVerifyNode.end()){
                res.first=ite->first;
                res.second=ite->second;
                return true;
            }
            return false;
        }

     private:
        std::map<std::string,Vrf> vrfCache;
        std::map<std::string,std::vector<std::string>> vrfVerifyNode;
        std::shared_mutex vrfInfoMutex;
        std::shared_mutex vrfNodeMutex;

        long long stringToll(const std::string& data)
        {
            long long value = 0;
            for(int i = 0;i< data.size() ;i++)
            {
                    int a= (int )data[i];
                    value += a;
            }
            return value;
        }
};






#endif