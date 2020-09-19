//
// Created by asus-lubuntu on 11/09/2020.
//

#include "block.h"

// TODO: copy/move over instead of passing entire string
Block::Block(const string& db_id, const string& doc_id, const string& doc, const string& sign, const int& proof_of_work, const string& prv_hash) :
                                                                            timestamp {chrono::system_clock::to_time_t(chrono::system_clock::now())},
                                                                             database_id {db_id},
                                                                             document_id {doc_id},
                                                                             document {doc},
                                                                             proof {proof_of_work},
                                                                             previous_hash {prv_hash}
{
    version = 1;
    active = true;
    signature = sign;
}

Block::Block(const Block& b, const string& doc, const int& proof_of_work, const string& prv_hash) :
                                timestamp {chrono::system_clock::to_time_t(chrono::system_clock::now())},
                                database_id {b.database_id},
                                document_id {b.document_id},
                                document {doc},
                                proof {proof_of_work},
                                previous_hash {prv_hash}

{
    version = b.version+1;
    active = b.active;
    signature = b.signature;
}

Block::Block(const Block& b, const string& doc, const int& proof_of_work, const string& prv_hash, const string& sign) :
        timestamp {chrono::system_clock::to_time_t(chrono::system_clock::now())},
        database_id {b.database_id},
        document_id {b.document_id},
        document {doc},
        proof {proof_of_work},
        previous_hash {prv_hash}

{
    version = b.version+1;
    active = b.active;
    signature = sign;
}

// Block functions

/*void Block::update_document(const string &doc) {
    if (active) {
        document = doc;
        ++version;
    }
}*/

/*void Block::restore_document(const string& doc) {
    if (active) {
        document = doc;
        ++version;
    }
}*/

void Block::resurrect_document() {
    if (!active){
        active = true;
    }
}

void Block::delete_document() {
    if (active) {
        active = false;
    }
}

// Block functions
string Block::hash_block() {
    string block_str = this->block_to_string();
    return picosha2::hash256_hex_string(block_str.begin(), block_str.end());
}

/*const string* Block::get_document() {
    const string* doc = &document;
    return doc;
}*/


