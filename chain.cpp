//
// Created by asus-lubuntu on 11/09/2020.
//

// TODO: Replace string with char*[] to fit with C?

#include "chain.h"

// Chain functions

Chain::Chain() {
    //Create genesis block
    string db_id = uid();
    string doc_id = uid();
    string doc = "'I think computer viruses should count as life ... I think it says something about human nature that the only form of life we have created so far is purely destructive. We've created life in our own image.'\n"
                 "― Stephen Hawking ";
    string signature = "open";
    string previous_hash = "0";
    int proof = 1;
    Block genesis_block{db_id, doc_id, doc, signature, proof, previous_hash};
    chain.push_back(genesis_block);
}

bool Chain::is_chain_valid() {
    for (int i = 1; i < chain.size(); ++i){
        Block previous_block = chain[i-1];
        Block current_block = chain[i];
        if (!current_block.check_block_validity(previous_block.hash_block())){
            return false;
        }
        int previous_proof = previous_block.get_proof();
        int current_proof = current_block.get_proof();
        // TODO: Change the calc, see proof of work
        int hash_calc = (current_proof + 1) - (previous_proof + 1);
        string hash_str = to_string(hash_calc);
        string hash_operation = picosha2::hash256_hex_string(hash_str.begin(), hash_str.end());
        string hash_head = hash_operation.substr(0,4);
        if (hash_head != "0000"){
            return false;
        }
    }
    return true;
}

void Chain::update_chain(Block block) {
// TODO: Another node makes an update chain call with a block telling it to a certain block to it, this will then check
//  whether this new block is valid or not, add it then check whether the chain is still valid, then tell everyone around
//  to do the same.
}

void Chain::replace_chain(Chain &new_chain) {
    //TODO later when everything else works
}

Block &Chain::find_block(const string& db_id, const string& doc_id) {
    //TODO: * pass location of where the docs are rather than the actual docs
    //      * implement a better searching algorithm
    //Reverse search to get latest block
    for (int i = chain.size(); i != 0; --i){
        if (chain[i].check_block(db_id, doc_id)){
            return chain[i];
        }
    }
}

Block &Chain::find_block(const string& db_id, const string& doc_id, const int& version) {
    //TODO: * pass location of where the docs are rather than the actual docs
    //      * implement a better searching algorithm
    for (Block& b : chain){
        if (b.check_block(db_id, doc_id, version)){
            return b;
        }
    }
}

vector<Block> Chain::find_blocks(const string& db_id) {
    //TODO: * pass location of where the docs are rather than the actual docs
    //      * implement a better searching algorithm
    vector<Block> blocks;
    for (Block& b : chain){
        if (b.check_block(db_id)){
            blocks.push_back(b);
        }
    }
    return blocks;
}

vector<Block> Chain::find_blocks(const string& db_id, const string& doc_id) {
    //TODO: * pass location of where the docs are rather than the actual docs
    //      * implement a better searching algorithm
    vector<Block> blocks;
    for (Block& b : chain){
        if (b.check_block(db_id, doc_id)){
            blocks.push_back(b);
        }
    }
    return blocks;
}

int Chain::proof_of_work(const int& previous_proof) {
    int new_proof = 1;
    bool check_proof = false;
    while(!check_proof){
        // int hash_calc = new_proof ** 2 - previous_proof ** 2
        int hash_calc = (new_proof + 1) - (previous_proof + 1);
        string hash_str = to_string(hash_calc);
        string hash_operation = picosha2::hash256_hex_string(hash_str.begin(), hash_str.end());
        string hash_head = hash_operation.substr(0,4);
        if (hash_head == "0000"){
            check_proof = true;
        }
        else {
            ++new_proof;
        }
    }
    return new_proof;
}

string Chain::previous_hash() {
    // hash the entire previous block;
    return chain.back().hash_block();
}

int Chain::previous_proof() {
    return chain.back().get_proof();
}

// Document functions
//  Create Document
string Chain::create_document(const string& db_id, const string& doc, const string& sign) {
    // TODO: check create on db permissions somehow
    string doc_id = uid();
    string prev_hash = previous_hash();
    int proof = proof_of_work(previous_proof());
    Block new_block{db_id, doc_id, doc, sign, proof, prev_hash};
    chain.push_back(new_block);
    return doc_id;
}

// Read document
string Chain::get_latest_document(const string& db_id, const string& doc_id) {
    // TODO: check the signature and key
    Block block = find_block(db_id, doc_id);
    string document = block.get_document();
    return document;
}

string Chain::get_document_version(const string& db_id, const string& doc_id, const int& version) {
    Block block = find_block(db_id, doc_id, version);
    string document = block.get_document();
    return document;
}

vector<string> Chain::get_all_document_versions(const string& db_id, const string& doc_id) {
    //TODO: pass location of where the docs are rather than the actual docs
    vector<string> all_document_versions;

    // TODO: See which implementation is the quickest
    /*Block first_block = find_block(db_id, doc_id, 0);
    all_document_versions.push_back(first_block.get_document());

    Block final_block = find_block(db_id, doc_id);
    int max_version = final_block.get_version();

    for (int i = 1; i < max_version; ++i){
        Block block = find_block(db_id, doc_id, i);
        all_document_versions.push_back(block.get_document());
    }
    all_document_versions.push_back(final_block.get_document());*/

    vector<Block> blocks = find_blocks(db_id, doc_id);

    for(Block& b : blocks){
        all_document_versions.push_back(b.get_document());
    }

    return all_document_versions;
}

vector<string> Chain::get_all_documents(const string& db_id) {
    //TODO: pass location of where the docs are rather than the actual docs
    vector<string> all_documents;

    vector<Block> blocks = find_blocks(db_id);

    for(Block& b : blocks){
        all_documents.push_back(b.get_document());
    }

    return all_documents;
}

// Update document
void Chain::update_document(string db_id, string doc_id, string doc) {
    // TODO: check the signature and key
    Block block = find_block(db_id, doc_id);
    // TODO: new block needs to be created... and added to the chain, implement a copy operator...
    block.update_document(doc);
}

void Chain::restore_document(string db_id, string doc_id, int version) {
    // TODO: check the signature and key
    Block current_block = find_block(db_id, doc_id);
    Block restore_block = find_block(db_id, doc_id, version);
    current_block.restore_document(restore_block.get_document());
}

void Chain::resurrect_document(string db_id, string doc_id) {
    // TODO: check the signature and key
    Block block = find_block(db_id, doc_id);
    block.resurrect_document();
}

// Delete document
void Chain::delete_document(string db_id, string doc_id) {
    // TODO: check the signature and key
    Block block = find_block(db_id, doc_id);
    block.delete_document();
}

bool Chain::is_document_active(const string& db_id, const string& doc_id) {
    Block block = find_block(db_id, doc_id);
    return block.is_active();
}


