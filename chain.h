//
// Created by asus-lubuntu on 11/09/2020.
//

#ifndef CPP_BLOCKCHAIN_DB_CHAIN_H
#define CPP_BLOCKCHAIN_DB_CHAIN_H

#include <vector>
#include <algorithm>
#include "block.h"
#include "picosha2.h"
#include "unique_id.h"

using namespace std;

class Chain {
    vector<Block> chain;
    Unique_id uid;

    // Chain functions
    Block& find_block(const string& db_id, const string& doc_id);
    Block& find_block(const string& db_id, const string& doc_id, const int& version);
    vector<Block> find_blocks(const string& db_id);
    vector<Block> find_blocks(const string& db_id, const string& doc_id);
    static int proof_of_work(const int& previous_proof);
    string previous_hash();
    int previous_proof();

    // Document functions

public:
    // Chain specific functions
    Chain();
    bool is_chain_valid();
    void replace_chain(Chain &chain);
    void update_chain(Block block);

    // Block specific functions
    // Create
    string create_document(const string& db_id, const string& doc, const string& sign);
    // Read
    string get_latest_document(const string& db_id, const string& doc_id);
    string get_document_version(const string& db_id, const string& doc_id, const int& version);
    vector<string> get_all_document_versions(const string& db_id, const string& doc_id);
    vector<string> get_all_documents(const string& db_id);
    bool is_document_active(const string& db_id, const string& doc_id);
    //Update
    // TODO: Should the ID change on update?
    void update_document(string db_id, string doc_id, string doc);
    // TODO: update encryption where the signature and doc change
    void restore_document(string db_id, string doc_id, int version);
    void resurrect_document(string db_id, string doc_id);

    //Delete
    void delete_document(string db_id, string doc_id);
};

#endif //CPP_BLOCKCHAIN_DB_CHAIN_H