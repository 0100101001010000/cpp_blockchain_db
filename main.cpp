#include <iostream>
#include "unique_id.h"
#include "chain.h"
#include <cassert>

int main() {
    // TODO:    * Deal with nodes here? or leave it to python wrapper?
    //          * Update network?
    //          * Let python wrapper deal with the signature and private keys?
    //          * Not necessarily comfortable with sending doc_id
    //          * Add update_encryption function where the signature and doc gets updated
    //          * Wrapper needs to deal with the checking of signature and key
    //          * Delete chain? Probably will help with the replace chain
    //          * Test replacing one of these chains with the other

    cout << "Starting tests" <<endl;

    // Create the chain

    cout << "Test 1, creating chain, adding a document and updating it." << endl;
    Chain first_chain;
    Unique_id uid;

    //Create a new document
    string db_id = uid();
    string document = uid();
    string signature = uid();
    // create document
    chrono::steady_clock::time_point doc_creation_start = std::chrono::steady_clock::now();
    string doc_id = first_chain.create_document(db_id, document, signature);
    //get latest doc to ensure that it aligns with what was sent
    string doc = first_chain.get_latest_document(db_id, doc_id);
    chrono::steady_clock::time_point doc_creation_end = std::chrono::steady_clock::now();
    assert(document == doc);
    cout << "Document creations works" << endl;
    auto creation_runtime = chrono::duration_cast<std::chrono::milliseconds>(doc_creation_end - doc_creation_start).count();
    cout << "It took " << creation_runtime << "ms to create a retrieve document" << endl;

    //create multiple versions
    vector<string> document_versions;
    document_versions.push_back(document);
    chrono::steady_clock::time_point doc_versions_start = std::chrono::steady_clock::now();
    for (int j = 0; j < 10; ++j){
        string document2 = uid();
        chrono::steady_clock::time_point update_start = std::chrono::steady_clock::now();
        first_chain.update_document(db_id, doc_id, document2);
        chrono::steady_clock::time_point update_end = std::chrono::steady_clock::now();
        string doc2 = first_chain.get_latest_document(db_id, doc_id);
        assert(document2 == doc2);
        document_versions.push_back(document2);
        auto update_runtime = chrono::duration_cast<std::chrono::milliseconds>(update_end - update_start).count();
        cout << "It took " << update_runtime << "ms to update a doc" << endl;
    }
    chrono::steady_clock::time_point doc_versions_end = std::chrono::steady_clock::now();
    cout << "Document updates worked" << endl;
    auto versions_runtime = chrono::duration_cast<std::chrono::milliseconds>(doc_versions_end - doc_versions_start).count();
    cout << "It took " << versions_runtime << "ms to create 10 doc versions" << endl;

    // go through retrieved document_versions and confirm that the are correct
    chrono::steady_clock::time_point versions_retrieval_start = std::chrono::steady_clock::now();
    for (int k = 1; k <= 10; ++k){
        //doc versions start at 1 but index vector index starts at 0 hence the k-1 below
        string doc_version = first_chain.get_document_version(db_id, doc_id, k);
        assert(document_versions[k-1] == doc_version);
    }
    chrono::steady_clock::time_point versions_retrieval_end = std::chrono::steady_clock::now();
    cout << "Document updates really worked" << endl;
    auto version_retrieval_runtime = chrono::duration_cast<std::chrono::nanoseconds>(versions_retrieval_end - versions_retrieval_start).count();
    cout << "It took " << version_retrieval_runtime << "ns to get 10 doc versions" << endl;

    // get all versions and compare
    chrono::steady_clock::time_point doc_retrieval_start = std::chrono::steady_clock::now();
    vector<string> all_document_versions = first_chain.get_all_document_versions(db_id, doc_id);
    chrono::steady_clock::time_point doc_retrieval_end = std::chrono::steady_clock::now();
    assert(document_versions == all_document_versions);
    cout << "Retrieving all document versions worked" << endl;
    auto doc_retrieval_runtime = chrono::duration_cast<std::chrono::nanoseconds>(doc_retrieval_end - doc_retrieval_start).count();
    cout << "It took " << doc_retrieval_runtime << "ns to get all versions" << endl;

    // restore a version 5
    chrono::steady_clock::time_point restore_start = std::chrono::steady_clock::now();
    first_chain.restore_document(db_id, doc_id, 5);
    chrono::steady_clock::time_point restore_end = std::chrono::steady_clock::now();
    string doc_v5 = first_chain.get_latest_document(db_id, doc_id);
    assert(document_versions[4] == doc_v5);
    cout << "Document restoration worked" << endl;
    auto restore_runtime = chrono::duration_cast<std::chrono::milliseconds>(restore_end - restore_start).count();
    cout << "It took " << restore_runtime << "ms to restore a doc" << endl;


    // delete a document
    chrono::steady_clock::time_point delete_start = std::chrono::steady_clock::now();
    first_chain.delete_document(db_id, doc_id);
    chrono::steady_clock::time_point delete_end = std::chrono::steady_clock::now();
    bool doc_active = first_chain.is_document_active(db_id, doc_id);
    assert(!doc_active);
    cout << "Document deletion worked" << endl;
    auto delete_runtime = chrono::duration_cast<std::chrono::milliseconds>(delete_end - delete_start).count();
    cout << "It took " << delete_runtime << "ms to delete a doc" << endl;

    // resurrect a document
    chrono::steady_clock::time_point resurrect_start = std::chrono::steady_clock::now();
    first_chain.resurrect_document(db_id, doc_id);
    chrono::steady_clock::time_point resurrect_end = std::chrono::steady_clock::now();
    bool doc_resurrected = first_chain.is_document_active(db_id, doc_id);
    assert(doc_resurrected);
    cout << "Document resurrection worked" << endl;
    auto resurrect_runtime = chrono::duration_cast<std::chrono::milliseconds>(resurrect_end - resurrect_start).count();
    cout << "It took " << resurrect_runtime << "ms to resurrect a doc" << endl;
    cout << "End of test 1" << endl;

    // Test get all docs
    cout << "Test 2: Getting all of the docs from a db" << endl;
    Chain second_chain;
    vector<string> db_docs;
    string db_id2 = uid();
    cout << "Creating docs" << endl;
    for (int i = 0; i < 10; ++i){
        string document2 = uid();
        string signature2 = uid();
        string doc_id2 = second_chain.create_document(db_id2, document2, signature2);
        for (int j = 0; j < 5; ++j){
            string document3 = uid();
            second_chain.update_document(db_id2, doc_id2, document3);
            if(j==4){
                db_docs.push_back(document3);
            }
        }
    }
    cout << "Docs created" << endl;
    chrono::steady_clock::time_point all_db_docs_start = std::chrono::steady_clock::now();
    vector<string> latest_db_docs = second_chain.get_all_documents(db_id2);
    chrono::steady_clock::time_point all_db_docs_end = std::chrono::steady_clock::now();
    assert(db_docs == latest_db_docs);
    cout << "All docs successfully retrieved" << endl;
    auto all_db_docs_runtime = chrono::duration_cast<std::chrono::nanoseconds>(all_db_docs_end - all_db_docs_start).count();
    cout << "It took " << all_db_docs_runtime << "ns to all db docs" << endl;
    cout << "End of Test 2" << endl;

    // track how much time it takes to create documents on new chains (1, 10, 100, 1000, 10000, 100000, 1000000)
    cout << "Timed tests" << endl;
    Chain timed_chain_1;
    string db_id_timed = uid();
    string document_timed = uid();
    string signature_timed = uid();

    // time to run 1:
    cout << "Booking 1 doc" << endl;
    chrono::steady_clock::time_point start_1 = std::chrono::steady_clock::now();
    timed_chain_1.create_document(db_id_timed, document_timed, signature_timed);
    chrono::steady_clock::time_point end_1 = std::chrono::steady_clock::now();
    auto runtime_1 = chrono::duration_cast<std::chrono::milliseconds>(end_1 - start_1).count();
    cout << "time to book 1: " << runtime_1 << endl;

    //time to run 10
    cout << "Booking 10 docs" << endl;
    chrono::steady_clock::time_point start_10 = std::chrono::steady_clock::now();
    for(int i=0; i < 10; ++i){
        timed_chain_1.create_document(db_id_timed, document_timed, signature_timed);
    }
    chrono::steady_clock::time_point end_10 = std::chrono::steady_clock::now();
    auto runtime_10 = chrono::duration_cast<std::chrono::milliseconds>(end_10 - start_10).count();
    cout << "time to book 10: " << runtime_10 << endl;
    cout << "Average time for 1 doc: " << runtime_10 / 10 << endl;

    //time to run 100
    cout << "Booking 100 docs" << endl;
    chrono::steady_clock::time_point start_100 = std::chrono::steady_clock::now();
    for(int i=0; i < 100; ++i){
        timed_chain_1.create_document(db_id_timed, document_timed, signature_timed);
    }
    chrono::steady_clock::time_point end_100 = std::chrono::steady_clock::now();
    auto runtime_100 = chrono::duration_cast<std::chrono::milliseconds>(end_100 - start_100).count();
    cout << "time to book 100: " << runtime_100 << endl;
    cout << "Average time for 1 doc: " << runtime_100 / 100 << endl;

    //time to run 1000
    cout << "Booking 1000 docs" << endl;
    chrono::steady_clock::time_point start_1000 = std::chrono::steady_clock::now();
    for(int i=0; i < 1000; ++i){
        timed_chain_1.create_document(db_id_timed, document_timed, signature_timed);
    }
    chrono::steady_clock::time_point end_1000 = std::chrono::steady_clock::now();
    auto runtime_1000 = chrono::duration_cast<std::chrono::milliseconds>(end_1000 - start_1000).count();
    cout << "time to book 1000: " << runtime_1000 << endl;
    cout << "Average time for 1 doc: " << runtime_1000 / 1000 << endl;

    //time to run 10000
    /*chrono::steady_clock::time_point start_10000 = std::chrono::steady_clock::now();
    for(int i=0; i < 10000; ++i){
        timed_chain_1.create_document(db_id_timed, document_timed, signature_timed);
    }
    chrono::steady_clock::time_point end_10000 = std::chrono::steady_clock::now();
    auto runtime_10000 = chrono::duration_cast<std::chrono::milliseconds>(end_10000 - start_10000).count();
    cout << "time to book 10000: " << runtime_10000 << endl;

    //time to run 100000
    chrono::steady_clock::time_point start_100000 = std::chrono::steady_clock::now();
    for(int i=0; i < 100000; ++i){
        timed_chain_1.create_document(db_id_timed, document_timed, signature_timed);
    }
    chrono::steady_clock::time_point end_100000 = std::chrono::steady_clock::now();
    auto runtime_100000 = chrono::duration_cast<std::chrono::milliseconds>(end_100000 - start_100000).count();
    cout << "time to book 100000: " << runtime_100000 << endl;*/

    // do maths to figure out whether the time increases with size
    // do doc retrievals on the various chains and track the time
    return 0;
}
