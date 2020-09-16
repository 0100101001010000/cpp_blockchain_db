//
// Block to store a document, includes db key, doc key, proof...
//

#ifndef CPP_BLOCKCHAIN_DB_BLOCK_H
#define CPP_BLOCKCHAIN_DB_BLOCK_H

#include <string>
#include <chrono>
#include <ctime>
#include "picosha2.h"

using namespace std;

class Block {
    // Block specific variables
    const time_t timestamp;
    const int proof;
    const string previous_hash;

    // Document specific variables
    const string database_id;
    const string document_id;
    string document;
    int version;
    bool active;
    string signature;

    // Block specific functions
    string block_to_string() {return to_string(timestamp) + "|" + to_string(proof) + "|" + previous_hash + "|" + database_id + "|" + document_id + "|" + to_string(version) + "|" + to_string(active) + "|" + signature + "|" + document; }

public:
    Block(const string& db_id, const string& doc_id, const string& doc, const string& sign, const int& proof_of_work, const string& prv_hash);

    // Block functions
    bool check_block(const string& db_id) {return database_id == db_id;}
    bool check_block(const string& db_id, const string& doc_id) {return database_id == db_id && document_id == doc_id;}
    bool check_block(const string& db_id, const string& doc_id, int ver) {return database_id == db_id && document_id == doc_id && version == ver;}
    int get_proof() const {return proof;}
    bool check_block_validity(string hash) {return previous_hash == hash;}
    string hash_block();

    // Document functions
    string get_document();
    void update_document(const string& doc);
    void restore_document(const string& doc);
    void resurrect_document();
    void delete_document();
    bool is_active() const{return active;}
};

#endif //CPP_BLOCKCHAIN_DB_BLOCK_H