#include "WebApi.h"
#include "IpfsAdapter.h"
#include "Database.h"

#include <Windows.h>
#include <iostream>

int main(int argc, char** argv)
{
	// Clear everything
	DeleteFileA("database.db");
	DeleteDirectory("E:\\Ipfs");
	Sleep(100);
	CreateDirectoryA("E:\\Ipfs", NULL);

	// Init
	Database::Init("database.db");

	// Add mt root
	string mt_root = "29837569183465864871254";
	Database::Mtroot mtroot(mt_root);
	mtroot.Save();

	// Add some stores
	ifstream fin("store1.txt", ios::binary);
	string store_id = "1234567890";
	string store_descr = "teststore";
	string store_pub_key = "1234567890";
	string store_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	string store_data_hash = Hash(store_data);
	Database::Store store1(store_id, store_descr, store_pub_key, store_data_hash);
	bool result = store1.Save();
	IpfsAdapter::Save(store_data_hash, store_data);
	fin.close();

	fin.open("store2.txt", ios::binary);
	store_id = "1234567891";
	store_descr = "teststore";
	store_pub_key = "1234567890";
	store_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	store_data_hash = Hash(store_data);
	Database::Store store2(store_id, store_descr, store_pub_key, store_data_hash);
	result = store2.Save();
	IpfsAdapter::Save(store_data_hash, store_data);
	fin.close();

	fin.open("store3.txt", ios::binary);
	store_id = "1234567892";
	store_descr = "teststore";
	store_pub_key = "1234567890";
	store_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	store_data_hash = Hash(store_data);
	Database::Store store3(store_id, store_descr, store_pub_key, store_data_hash);
	result = store3.Save();
	IpfsAdapter::Save(store_data_hash, store_data);
	fin.close();

	fin.open("store4.txt", ios::binary);
	store_id = "1234567893";
	store_descr = "teststore";
	store_pub_key = "1234567890";
	store_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	store_data_hash = Hash(store_data);
	Database::Store store4(store_id, store_descr, store_pub_key, store_data_hash);
	result = store4.Save();
	IpfsAdapter::Save(store_data_hash, store_data);
	fin.close();

	fin.open("store5.txt", ios::binary);
	store_id = "1234567894";
	store_descr = "teststore";
	store_pub_key = "1234567890";
	store_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	store_data_hash = Hash(store_data);
	Database::Store store5(store_id, store_descr, store_pub_key, store_data_hash);
	result = store5.Save();
	IpfsAdapter::Save(store_data_hash, store_data);
	fin.close();

	// Add a pair of items (and 4 more)
	fin.open("item1.txt", ios::binary);
	string item_id = "1623489162384716234891623847";
	string item_store_id = store2.m_id;
	string item_descr = "item1";
	string item_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	string item_data_hash = Hash(item_data);
	Database::Item item1(item_id, item_store_id, item_descr, item_data_hash);
	result = item1.Save();
	IpfsAdapter::Save(item_data_hash, item_data);
	fin.close();

	fin.open("item2.txt", ios::binary);
	item_id = "1623489162384816234891623848";
	item_store_id = store2.m_id;
	item_descr = "item2";
	item_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	item_data_hash = Hash(item_data);
	Database::Item item2(item_id, item_store_id, item_descr, item_data_hash);
	result = item2.Save();
	IpfsAdapter::Save(item_data_hash, item_data);
	fin.close();

	fin.open("item3.txt", ios::binary);
	item_id = "1623489162384916234891623849";
	item_store_id = store2.m_id;
	item_descr = "item2";
	item_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	item_data_hash = Hash(item_data);
	Database::Item item3(item_id, item_store_id, item_descr, item_data_hash);
	result = item3.Save();
	IpfsAdapter::Save(item_data_hash, item_data);
	fin.close();

	fin.open("item4.txt", ios::binary);
	item_id = "1623489162385016234891623850";
	item_store_id = store2.m_id;
	item_descr = "item2";
	item_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	item_data_hash = Hash(item_data);
	Database::Item item4(item_id, item_store_id, item_descr, item_data_hash);
	result = item4.Save();
	IpfsAdapter::Save(item_data_hash, item_data);
	fin.close();

	fin.open("item5.txt", ios::binary);
	item_id = "1623489162385116234891623851";
	item_store_id = store2.m_id;
	item_descr = "item2";
	item_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	item_data_hash = Hash(item_data);
	Database::Item item5(item_id, item_store_id, item_descr, item_data_hash);
	result = item5.Save();
	IpfsAdapter::Save(item_data_hash, item_data);
	fin.close();

	fin.open("item6.txt", ios::binary);
	item_id = "1623489162385216234891623852";
	item_store_id = store2.m_id;
	item_descr = "item2";
	item_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	item_data_hash = Hash(item_data);
	Database::Item item6(item_id, item_store_id, item_descr, item_data_hash);
	result = item6.Save();
	IpfsAdapter::Save(item_data_hash, item_data);
	fin.close();

	// Add some users
	string user_id = "1724326321326253321";
	int user_ordinal = 0;
	Database::User user1(user_id, user_ordinal);
	user1.Save();

	user_id = "2358795423789198236";
	user_ordinal = 1;
	Database::User user2(user_id, user_ordinal);
	user2.Save();

	user_id = "38797826286434363427";
	user_ordinal = 2;
	Database::User user3(user_id, user_ordinal);
	user3.Save();

	user_id = "4782995834270543126";
	user_ordinal = 3;
	Database::User user4(user_id, user_ordinal);
	user4.Save();

	// Add some comments
	fin.open("comment1.txt", ios::binary);
	string comment_id = "523985423598723474573";
	string comment_item_id = item5.m_id;
	string comment_mt_root = mt_root;
	string comment_proof = "78245641783658273494829";
	string comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	string comment_data_hash = Hash(comment_data);
	Database::Comment comment1(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment1.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment2.txt", ios::binary);
	comment_id = "19743591723458912630847";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment2(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment2.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment3.txt", ios::binary);
	comment_id = "19743591723458912630848";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment3(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment3.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment4.txt", ios::binary);
	comment_id = "19743591723458912630849";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment4(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment4.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment5.txt", ios::binary);
	comment_id = "19743591723458912630850";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment5(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment5.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment6.txt", ios::binary);
	comment_id = "19743591723458912630851";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment6(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment6.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment7.txt", ios::binary);
	comment_id = "19743591723458912630852";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment7(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment7.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment8.txt", ios::binary);
	comment_id = "19743591723458912630853";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment8(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment8.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment9.txt", ios::binary);
	comment_id = "19743591723458912630854";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment9(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment9.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment10.txt", ios::binary);
	comment_id = "19743591723458912630855";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment10(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment10.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment11.txt", ios::binary);
	comment_id = "19743591723458912630856";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment11(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment11.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	fin.open("comment12.txt", ios::binary);
	comment_id = "19743591723458912630857";
	comment_item_id = item5.m_id;
	comment_mt_root = mt_root;
	comment_proof = "810968237956437826523";
	comment_data = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	comment_data_hash = Hash(comment_data);
	Database::Comment comment12(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
	comment12.Save();
	IpfsAdapter::Save(comment_data_hash, comment_data);
	fin.close();

	//WebApi::Init();
	StartWebApi(argc, argv);
	Sleep(INFINITE);

}
