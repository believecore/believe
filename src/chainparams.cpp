// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Believe developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000003a2f6510491d6a72068d3a7dea5de22001cfc0f32f8185390b9db1c426f"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1561939200, // * UNIX timestamp of last checkpoint block
    0,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x5a;
        pchMessageStart[1] = 0x41;
        pchMessageStart[2] = 0x9c;
        pchMessageStart[3] = 0xb3;
        vAlertPubKey = ParseHex("044d3297dbed0fe340d9c83cada97ca32be26fe83f1e949862030d62fdb253062f3f7c529bacbb43a7f1f223ea738de1c2b8e702b6c2354ded1c48a607dd633041");
        nDefaultPort = 33672;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nSubsidyHalvingInterval = 2102400;// 4years:blocktime 1min 525600 block/year
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Believe: 1 minute 
        nTargetSpacing = 2 * 60;  // Believe: 2 minutes
        nMaturity = 10;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 27000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200;
        nModifierUpdateBlock = 1; // we use the version 2 for BLE

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * genesis.py -a quark-hash -z "Even With Energy Surplus, Canada Unable to Meet Electricity Demands of Bitcoin Miners" -t 1516926684 -v 0 -p 04e5a8143f86ad8ac63791fbbdb8e0b91a8da88c8c693a95f6c2c13c063ea790f7960b8025a9047a7bc671d5cfe707a2dd2e13b86182e1064a0eea7bf863636363
         * 04ffff001d01042642544320426c6f636b20353031353932202d20323031372d31322d32392031353a34333a3337
         * algorithm: quark-hash
         * merkle hash: 07cbcacfc822fba6bbeb05312258fa43b96a68fc310af8dfcec604591763f7cf
         * pszTimestamp: Even With Energy Surplus, Canada Unable to Meet Electricity Demands of Bitcoin Miners
         * pubkey: 04e5a8143f86ad8ac63791fbbdb8e0b91a8da88c8c693a95f6c2c13c063ea790f7960b8025a9047a7bc671d5cfe707a2dd2e13b86182e1064a0eea7bf863636363
         * time: 1516926684
         * bits: 0x1e0ffff0
         * Searching for genesis hash..
         * 16525.0 hash/s, estimate: 72.2 hgenesis hash found!
         * nonce: 21256609
         * genesis_hash: 000008467c3a9c587533dea06ad9380cded3ed32f9742a6c0c1aebc21bf2bc9b
         */
        const char* pszTimestamp = "Successful Believe Blockchain by VYP";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0492644455c952c9ab22232464e237c599f291146bc17793cd79c0a6b3d2bf64648e4e1d5414f448b8ea5ed1fd6fc854c5adad7a7cb876b7e1aa85d2ed19493290") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1561939200;// 7/1/2019 @ 12:00am (UTC)
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 2177672;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000003a2f6510491d6a72068d3a7dea5de22001cfc0f32f8185390b9db1c426f"));
        assert(genesis.hashMerkleRoot == uint256("0x540bf2ad12c6e7c342462ec8ab5a281dd5cc06749c87a5e9c568073a8ce95a3f"));

        // DNS Seeding
        vSeeds.push_back(CDNSSeedData("142.93.43.152", "142.93.43.152"));
        vSeeds.push_back(CDNSSeedData("167.99.80.246", "167.99.80.246"));
        vSeeds.push_back(CDNSSeedData("134.209.100.230", "134.209.100.230"));
        vSeeds.push_back(CDNSSeedData("159.89.107.205", "159.89.107.205"));

        // Believe addresses start with 'B'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);
        // Believe script addresses start with '3'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 6);
        // Believe private keys start with 'K'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 46);
        // Believe BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Believe BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Believe BIP44 coin type is '222' (0x800000de)
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xde).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "045e4ff4616b7683915e58cf58c087ed2c5301b15556798df91646cfef9df69c2ec766984e4388e1e05ba8a239c4573bd2c8178f39df973b7a6c2fbc96e00a3313";
        strMasternodePoolDummyAddress = "BHWQ9EfPAMVUHHHK87hFsTfPmPkvZ5EbYE";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis creation

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x33;
        pchMessageStart[1] = 0x8a;
        pchMessageStart[2] = 0xe5;
        pchMessageStart[3] = 0xa8;
        vAlertPubKey = ParseHex("04ba89975265af1d4c6295d3587eb4a0e4b758bde1621ef2ab8f92b98e7ed1c85547c9b7a3f145a66aa2abb91db5c13295828e77d823ea6d9b4bb89912425e1efe");
        nDefaultPort = 19333;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Believe: 1 day
        nTargetSpacing = 2 * 60;  // Believe: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 21000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1534570200;
        genesis.nNonce = 21148656;

        hashGenesisBlock = genesis.GetHash();
        // assert(hashGenesisBlock == uint256("0x000000938f4f20c6ccb3fea36539ade5af73d0bb45c55af64c7f7f1bfa5f3381"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet Believe addresses start with 'g'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 98);
        // Testnet Believe script addresses start with '5' or '6'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 12);
        // Testnet private keys start with 'k'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 108);
        // Testnet Believe BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Believe BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet believe BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "041bab97de321ccf1e78d10164f90bd87e3070ddb5586389013e9f15c4931e2451f318f1a8f177c5e9a3f5b4b13145f78e8116d2aa7ae14ef27f1f076b02ba852c";
        strMasternodePoolDummyAddress = "gbJ4Qad4xc77PpLzMx6rUegAs6aUPWkcUq";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x20;
        pchMessageStart[1] = 0xee;
        pchMessageStart[2] = 0x32;
        pchMessageStart[3] = 0xbc;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Believe: 1 day
        nTargetSpacing = 2 * 60;        // Believe: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1516926684;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 20542300;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 29333;
        // assert(hashGenesisBlock == uint256("0x229874aa8a92df3347600978e226ba57bc994b9fa291ea50519afafca2d50ed3"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
