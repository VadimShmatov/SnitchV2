#include "Database.h"

sqlite3* Database::db;

void Database::Init(string filename)
{
	if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
		FatalError(L"Error opening blockchain database");

	if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Mtroot(root BLOB PRIMARY KEY NOT NULL);", NULL, NULL, NULL) != SQLITE_OK)
		FatalError(L"Error creating table Mtroot");

	if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Store(id BLOB PRIMARY KEY NOT NULL, descr TEXT NOT NULL, pub_key BLOB NOT NULL, data_hash BLOB NOT NULL);", NULL, NULL, NULL) != SQLITE_OK)
		FatalError(L"Error creating table Store");

	if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Item(id BLOB PRIMARY KEY NOT NULL, store_id BLOB NOT NULL, descr TEXT NOT NULL, data_hash BLOB NOT NULL);", NULL, NULL, NULL) != SQLITE_OK)
		FatalError(L"Error creating table Item");

	if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS User(id BLOB PRIMARY KEY NOT NULL, ordinal INT NOT NULL UNIQUE);", NULL, NULL, NULL) != SQLITE_OK)
		FatalError(L"Error creating table User");

	if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Comment(id BLOB PRIMARY KEY NOT NULL, item_id BLOB NOT NULL, mt_root BLOB NOT NULL, proof BLOB NOT NULL, data_hash BLOB NOT NULL);", NULL, NULL, NULL) != SQLITE_OK)
		FatalError(L"Error creating table Comment");

}

Database::Mtroot::Mtroot(string root)
	: m_root(root)
{

}

bool Database::Mtroot::Save()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "INSERT INTO Mtroot(root) VALUES (?);", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (INSERT INTO Mtroot)");

	if (sqlite3_bind_blob(stmt, 1, m_root.data(), m_root.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Mtroot.root");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

bool Database::Mtroot::Delete()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "DELETE FROM Mtroot WHERE root = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (DELETE FROM Mtroot)");

	if (sqlite3_bind_blob(stmt, 1, m_root.data(), m_root.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Mtroot.root");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

bool Database::Mtroot::CheckExtsts(string root)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Mtroot WHERE root = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Mtroot)");

	if (sqlite3_bind_blob(stmt, 1, root.data(), root.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Mtroot.root");

	bool success = (sqlite3_step(stmt) == SQLITE_ROW);

	sqlite3_finalize(stmt);
	return success;
}

Database::Store::Store()
{

}

Database::Store::Store(string id, string descr, string pub_key, string data_hash)
	: m_id(id), m_descr(descr), m_pub_key(pub_key), m_data_hash(data_hash)
{

}

bool Database::Store::Save()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "INSERT INTO Store(id, descr, pub_key, data_hash) VALUES (?, ?, ?, ?);", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (INSERT INTO Store)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Store.id");

	if (sqlite3_bind_text(stmt, 2, m_descr.data(), m_descr.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding TEXT value Store.descr");

	if (sqlite3_bind_blob(stmt, 3, m_pub_key.data(), m_pub_key.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Store.pub_key");

	if (sqlite3_bind_blob(stmt, 4, m_data_hash.data(), m_data_hash.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Store.data_hash");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

bool Database::Store::Load(string id, Store& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Store WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Store)");

	if (sqlite3_bind_blob(stmt, 1, id.data(), id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Store.id");

	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		result.m_id = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		result.m_descr = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		result.m_pub_key = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		result.m_data_hash = string(buf, length);

		return true;
	}

	return false;
}

bool Database::Store::LoadAll(vector<Store>& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Store;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Store)");

	bool success = false;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		if (!success)
		{
			success = true;
			result.clear();
		}

		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		string id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		string descr(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		string pub_key(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		string data_hash(buf, length);

		result.push_back(Store(id, descr, pub_key, data_hash));
	}

	return success;
}

bool Database::Store::Find(string descr, vector<Store>& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Store WHERE descr LIKE ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Store)");

	descr = "%" + descr + "%";
	if (sqlite3_bind_text(stmt, 1, descr.data(), descr.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Store.descr");

	bool success = false;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		if (!success)
		{
			success = true;
			result.clear();
		}

		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		string id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		string descr(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		string pub_key(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		string data_hash(buf, length);

		result.push_back(Store(id, descr, pub_key, data_hash));
	}

	return success;
}

bool Database::Store::Delete()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "DELETE FROM Store WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (DELETE FROM Store)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Store.id");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

Database::Item::Item()
{

}

Database::Item::Item(string id, string store_id, string descr, string data_hash)
	: m_id(id), m_store_id(store_id), m_descr(descr), m_data_hash(data_hash)
{

}

bool Database::Item::Save()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "INSERT INTO Item(id, store_id, descr, data_hash) VALUES (?, ?, ?, ?);", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (INSERT INTO Item)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Item.id");

	if (sqlite3_bind_blob(stmt, 2, m_store_id.data(), m_store_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Item.store_id");

	if (sqlite3_bind_text(stmt, 3, m_descr.data(), m_descr.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding TEXT value Item.descr");

	if (sqlite3_bind_blob(stmt, 4, m_data_hash.data(), m_data_hash.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Item.data_hash");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

bool Database::Item::Load(string id, Item& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Item WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Item)");

	if (sqlite3_bind_blob(stmt, 1, id.data(), id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Item.id");

	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		result.m_id = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		result.m_store_id = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		result.m_descr = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		result.m_data_hash = string(buf, length);

		return true;
	}

	return false;
}

bool Database::Item::LoadAll(string store_id, vector<Item>& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Item WHERE store_id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Item)");

	if (sqlite3_bind_blob(stmt, 1, store_id.data(), store_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Item.store_id");

	bool success = false;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		if (!success)
		{
			success = true;
			result.clear();
		}

		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		string id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		string store_id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		string descr(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		string data_hash(buf, length);

		result.push_back(Item(id, store_id, descr, data_hash));
	}

	return success;
}

bool Database::Item::Find(string descr, vector<Item>& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Item WHERE descr LIKE ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Item)");

	descr = "%" + descr + "%";
	if (sqlite3_bind_text(stmt, 1, descr.data(), descr.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding TEXT value Item.descr");

	bool success = false;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		if (!success)
		{
			success = true;
			result.clear();
		}

		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		string id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		string store_id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		string descr(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		string data_hash(buf, length);

		result.push_back(Item(id, store_id, descr, data_hash));
	}

	return success;
}

bool Database::Item::Delete()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "DELETE FROM Item WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (DELETE FROM Item)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Item.id");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

Database::User::User(string id, int ordinal)
	: m_id(id), m_ordinal(ordinal)
{

}

bool Database::User::Save()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "INSERT INTO User(id, ordinal) VALUES (?, ?);", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (INSERT INTO User)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value User.id");

	if (sqlite3_bind_int(stmt, 2, m_ordinal) != SQLITE_OK)
		FatalError(L"Error binding INT value User.ordinal");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

bool Database::User::Load(string id, User& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM User WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM User)");

	if (sqlite3_bind_blob(stmt, 1, id.data(), id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value User.id");

	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		result.m_id = string(buf, length);

		result.m_ordinal = sqlite3_column_int(stmt, 1);

		return true;
	}

	return false;
}

bool Database::User::Delete()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "DELETE FROM User WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (DELETE FROM User)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value User.id");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

Database::Comment::Comment(string id, string item_id, string mt_root, string proof, string data_hash)
	: m_id(id), m_item_id(item_id), m_mt_root(mt_root), m_proof(proof), m_data_hash(data_hash)
{

}

bool Database::Comment::Save()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "INSERT INTO Comment(id, item_id, mt_root, proof, data_hash) VALUES (?, ?, ?, ?, ?);", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (INSERT INTO Comment)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.id");

	if (sqlite3_bind_blob(stmt, 2, m_item_id.data(), m_item_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.item_id");

	if (sqlite3_bind_blob(stmt, 3, m_mt_root.data(), m_mt_root.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.mt_root");

	if (sqlite3_bind_blob(stmt, 4, m_proof.data(), m_proof.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.proof");

	if (sqlite3_bind_blob(stmt, 5, m_data_hash.data(), m_data_hash.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.data_hash");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}

bool Database::Comment::CheckExists(string id)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Comment WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Comment)");

	if (sqlite3_bind_blob(stmt, 1, id.data(), id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.id");

	bool result = (sqlite3_step(stmt) == SQLITE_ROW);

	sqlite3_finalize(stmt);
	return result;
}

bool Database::Comment::Load(string id, Comment& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Comment WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Comment)");

	if (sqlite3_bind_blob(stmt, 1, id.data(), id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.id");

	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		result.m_id = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		result.m_item_id = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		result.m_mt_root = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		result.m_proof = string(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 4));
		length = sqlite3_column_bytes(stmt, 4);
		result.m_data_hash = string(buf, length);

		return true;
	}

	return false;
}

bool Database::Comment::LoadAll(string item_id, vector<Comment>& result)
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "SELECT * FROM Comment WHERE item_id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (SELECT FROM Comment)");

	if (sqlite3_bind_blob(stmt, 1, item_id.data(), item_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.item_id");

	bool success = false;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		if (!success)
		{
			success = true;
			result.clear();
		}

		const char *buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0));
		int length = sqlite3_column_bytes(stmt, 0);
		string id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 1));
		length = sqlite3_column_bytes(stmt, 1);
		string item_id(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 2));
		length = sqlite3_column_bytes(stmt, 2);
		string mt_root(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 3));
		length = sqlite3_column_bytes(stmt, 3);
		string proof(buf, length);

		buf = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 4));
		length = sqlite3_column_bytes(stmt, 4);
		string data_hash(buf, length);

		result.push_back(Comment(id, item_id, mt_root, proof, data_hash));
	}

	return success;
}

bool Database::Comment::Delete()
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(db, "DELETE FROM Comment WHERE id = ?;", -1, &stmt, 0) != SQLITE_OK)
		FatalError(L"Error preparing statement (DELETE FROM Comment)");

	if (sqlite3_bind_blob(stmt, 1, m_id.data(), m_id.length(), SQLITE_STATIC) != SQLITE_OK)
		FatalError(L"Error binding BLOB value Comment.id");

	bool result = (sqlite3_step(stmt) == SQLITE_DONE);
	sqlite3_finalize(stmt);
	return result;
}
