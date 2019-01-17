//#include "WebApi.h"
//
//thread WebApi::listening_thread;
//
//void WebApi::BuildResponse(const string& response_data, string& full_response)
//{
//	full_response = "HTTP/1.1 200 OK\r\n";
//	full_response += "Content-Length: " + to_string(response_data.size()) + "\r\n";
//	full_response += "Content-Type: application/json\r\n";
//	full_response += "Connection: Closed\r\n\r\n";
//	full_response += response_data;
//}
//
//void WebApi::ProcessRequest(const string& client_request, string& client_response)
//{
//	regex rgx;
//	if (client_request.length() > 4 && client_request.substr(0, 4) == "GET ")
//	{
//		rgx = "\\?([a-zA-Z0-9._~%&=:,{}-]*)";
//
//	}
//	else if (client_request.length() > 5 && client_request.substr(0, 4) == "POST ")
//	{
//		rgx = "\\x0D\\x0A\\x0D\\x0A([a-zA-Z0-9._~%&=:,{}-]*)";
//	}
//	else
//	{
//		BuildResponse("{\"error\": \"Request method not supported\"}", client_response);
//		return;
//	}
//
//	map<string, string> parameters;
//	smatch match;
//	if (regex_search(client_request, match, rgx))
//	{
//		string all_params = match[1];
//		rgx = "([a-zA-Z0-9%._~:,{}-]+)=([a-zA-Z0-9%._~:,{}-]+)";
//		while (regex_search(all_params, match, rgx))
//		{
//			parameters[UrlDecode(match[1])] = UrlDecode(match[2]);
//			all_params = match.suffix();
//		}
//	}
//
//	if (parameters.count("action") == 0)
//	{
//		BuildResponse("{\"error\": \"Action not specified\"}", client_response);
//		return;
//	}
//
//ResolveAction:
//	if (parameters["action"] == "ListStores")
//	{
//		vector<Database::Store> stores;
//		if (!Database::Store::LoadAll(stores))
//		{
//			BuildResponse("{\"error\": \"Cannot load list of stores\"}", client_response);
//			return;
//		}
//
//		json response;
//		response["action"] = "ListStores";
//		response["stores"] = json::array();
//
//		for (auto it = stores.begin(); it != stores.end(); it++)
//		{
//			json store_data;
//			string data;
//			if (!IpfsAdapter::Load(it->m_data_hash, data))
//			{
//				continue;
//			}
//			store_data = json::parse(data);
//			store_data["id"] = BinToHex(it->m_id);
//			response["stores"].push_back(store_data);
//		}
//
//		string res = response.dump();
//		BuildResponse(res, client_response);
//		return;
//	}
//	else if (parameters["action"] == "ListItems")
//	{
//		if (parameters.count("store_id") == 0)
//		{
//			BuildResponse("{\"error\": \"Required parameter missing: store_id\"}", client_response);
//			return;
//		}
//		string store_id = HexToBin(parameters["store_id"]);
//		Database::Store store;
//		if (!Database::Store::Load(store_id, store))
//		{
//			BuildResponse("{\"error\": \"Store not found\"}", client_response);
//			return;
//		}
//
//		vector<Database::Item> items;
//		if (!Database::Item::LoadAll(store_id, items))
//		{
//			BuildResponse("{\"error\": \"Cannot load list of items\"}", client_response);
//			return;
//		}
//
//		json response;
//		response["action"] = "ListItems";
//
//		string store_data;
//		if (IpfsAdapter::Load(store.m_data_hash, store_data))
//		{
//			response["store"] = json::parse(store_data);
//			response["store"]["id"] = BinToHex(store.m_id);
//		}
//
//		response["items"] = json::array();
//		for (auto it = items.begin(); it != items.end(); it++)
//		{
//			json item_data;
//			string data;
//			if (!IpfsAdapter::Load(it->m_data_hash, data))
//			{
//				continue;
//			}
//			item_data = json::parse(data);
//			item_data["id"] = BinToHex(it->m_id);
//			response["items"].push_back(item_data);
//		}
//
//		string res = response.dump();
//		BuildResponse(res, client_response);
//		return;
//	}
//	else if (parameters["action"] == "ListComments")
//	{
//		if (parameters.count("item_id") == 0)
//		{
//			BuildResponse("{\"error\": \"Required parameter missing: item_id\"}", client_response);
//			return;
//		}
//		string item_id = HexToBin(parameters["item_id"]);
//		Database::Item item;
//		if (!Database::Item::Load(item_id, item))
//		{
//			BuildResponse("{\"error\": \"Item not found\"}", client_response);
//			return;
//		}
//
//		vector<Database::Comment> comments;
//		if (!Database::Comment::LoadAll(item_id, comments))
//		{
//			BuildResponse("{\"error\": \"Cannot load list of comments\"}", client_response);
//			return;
//		}
//
//		json response;
//		response["action"] = "ListComments";
//
//		string item_data;
//		if (IpfsAdapter::Load(item.m_data_hash, item_data))
//		{
//			response["item"] = json::parse(item_data);
//			response["item"]["id"] = BinToHex(item.m_id);
//		}
//
//		response["comments"] = json::array();
//		for (auto it = comments.begin(); it != comments.end(); it++)
//		{
//			json comment_data;
//			string data;
//			if (!IpfsAdapter::Load(it->m_data_hash, data))
//			{
//				continue;
//			}
//			comment_data = json::parse(data);
//			comment_data["id"] = BinToHex(it->m_id);
//			response["comments"].push_back(comment_data);
//		}
//
//		string res = response.dump();
//		BuildResponse(res, client_response);
//		return;
//	}
//	else if (parameters["action"] == "AddComment")
//	{
//		if (parameters.count("item_id") == 0)
//		{
//			BuildResponse("{\"error\": \"Required parameter missing: item_id\"}", client_response);
//			return;
//		}
//		string item_id = HexToBin(parameters["item_id"]);
//
//		if (parameters.count("signature") == 0)
//		{
//			BuildResponse("{\"error\": \"Required parameter missing: signature\"}", client_response);
//			return;
//		}
//		string signature = HexToBin(parameters["signature"]);
//
//		if (parameters.count("comment_data") == 0)
//		{
//			BuildResponse("{\"error\": \"Required parameter missing: comment_data\"}", client_response);
//			return;
//		}
//		string comment_data = parameters["comment_data"];
//
//		string comment_id = item_id;
//		if (Database::Comment::CheckExists(comment_id))
//		{
//			BuildResponse("{\"error\": \"You have already created a comment\"}", client_response);
//			return;
//		}
//		string comment_item_id = item_id;
//		string comment_mt_root = "29837569183465864871254";
//		string comment_proof = "1956234756109823680498723";
//		string comment_data_hash = Hash(comment_data);
//		Database::Comment comment(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
//		comment.Save();
//		IpfsAdapter::Save(comment_data_hash, comment_data);
//
//		parameters["action"] = "ListComments";
//		goto ResolveAction;
//	}
//	else
//	{
//		BuildResponse("{\"error\": \"Action not supported\"}", client_response);
//	}
//
//	json result;
//	for (auto it = parameters.begin(); it != parameters.end(); it++)
//	{
//		result[it->first] = it->second;
//	}
//	string res = result.dump();
//	BuildResponse(res, client_response);
//}
//
//void WebApi::Listen()
//{
//	WSADATA wsaData;
//	int iResult;
//
//	SOCKET ListenSocket = INVALID_SOCKET;
//	SOCKET ClientSocket = INVALID_SOCKET;
//
//	struct addrinfo *result = NULL;
//	struct addrinfo hints;
//
//	int iSendResult;
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	// Initialize Winsock
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (iResult != 0) {
//		printf("WSAStartup failed with error: %d\n", iResult);
//		return;
//	}
//
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//	hints.ai_flags = AI_PASSIVE;
//
//	// Resolve the server address and port
//	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//	if (iResult != 0) {
//		printf("getaddrinfo failed with error: %d\n", iResult);
//		WSACleanup();
//		return;
//	}
//
//	// Create a SOCKET for connecting to server
//	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//	if (ListenSocket == INVALID_SOCKET) {
//		printf("socket failed with error: %ld\n", WSAGetLastError());
//		freeaddrinfo(result);
//		WSACleanup();
//		return;
//	}
//
//	// Setup the TCP listening socket
//	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
//	if (iResult == SOCKET_ERROR) {
//		printf("bind failed with error: %d\n", WSAGetLastError());
//		freeaddrinfo(result);
//		closesocket(ListenSocket);
//		WSACleanup();
//		return;
//	}
//
//	freeaddrinfo(result);
//
//	iResult = listen(ListenSocket, SOMAXCONN);
//	if (iResult == SOCKET_ERROR) {
//		printf("listen failed with error: %d\n", WSAGetLastError());
//		closesocket(ListenSocket);
//		WSACleanup();
//		return;
//	}
//
//	while (true)
//	{
//		// Accept a client socket
//		ClientSocket = accept(ListenSocket, NULL, NULL);
//		if (ClientSocket == INVALID_SOCKET) {
//			printf("accept failed with error: %d\n", WSAGetLastError());
//			closesocket(ListenSocket);
//			WSACleanup();
//			return;
//		}
//
//		u_long iMode = 1;
//		ioctlsocket(ClientSocket, FIONBIO, &iMode);
//
//		// No longer need server socket
//		//closesocket(ListenSocket);
//
//		// Receive until the peer shuts down the connection
//		string client_request;
//		do {
//			Sleep(100);
//			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
//			if (iResult > 0) {
//				printf("Bytes received: %d\n", iResult);
//
//				client_request += string(recvbuf, iResult);
//			}
//			else if (iResult == 0)
//			{
//				printf("Full request received, processing...\n", iResult);
//				string client_response;
//				ProcessRequest(client_request, client_response);
//
//				iSendResult = send(ClientSocket, client_response.data(), client_response.length(), 0);
//				if (iSendResult == SOCKET_ERROR) {
//					printf("send failed with error: %d\n", WSAGetLastError());
//					closesocket(ClientSocket);
//					WSACleanup();
//					return;
//				}
//				printf("Response sent\n", iResult);
//				printf("Connection closing...\n");
//			}
//			else {
//				int error = WSAGetLastError();
//				if (error != 10035)
//				{
//					printf("recv failed with error: %d\n", error);
//					closesocket(ClientSocket);
//					WSACleanup();
//					return;
//				}
//				else
//				{
//					printf("Full request received, processing...\n", iResult);
//					string client_response;
//					ProcessRequest(client_request, client_response);
//
//					iSendResult = send(ClientSocket, client_response.data(), client_response.length(), 0);
//					if (iSendResult == SOCKET_ERROR) {
//						printf("send failed with error: %d\n", WSAGetLastError());
//						closesocket(ClientSocket);
//						WSACleanup();
//						return;
//					}
//					printf("Response sent\n", iResult);
//					printf("Connection closing...\n");
//					iResult = 0;
//				}
//			}
//
//		} while (iResult > 0);
//
//		// shutdown the connection since we're done
//		iResult = shutdown(ClientSocket, SD_SEND);
//		if (iResult == SOCKET_ERROR) {
//			printf("shutdown failed with error: %d\n", WSAGetLastError());
//			closesocket(ClientSocket);
//			WSACleanup();
//			return;
//		}
//	}
//
//	// cleanup
//	closesocket(ClientSocket);
//	WSACleanup();
//}
//
//void WebApi::Init()
//{
//	listening_thread = thread(Listen);
//}

#include "WebApi.h"

class MyPartHandler : public Poco::Net::PartHandler
{
public:
	MyPartHandler() :
		_length(0)
	{
	}

	void handlePart(const MessageHeader& header, std::istream& stream)
	{
		_type = header.get("Content-Type", "(unspecified)");
		if (header.has("Content-Disposition"))
		{
			std::string disp;
			NameValueCollection params;
			MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
			_name = params.get("name", "(unnamed)");
			_fileName = params.get("filename", "(unnamed)");
		}

		CountingInputStream istr(stream);
		NullOutputStream ostr;
		StreamCopier::copyStream(istr, ostr);
		_length = istr.chars();
	}

	int length() const
	{
		return _length;
	}

	const std::string& name() const
	{
		return _name;
	}

	const std::string& fileName() const
	{
		return _fileName;
	}

	const std::string& contentType() const
	{
		return _type;
	}

private:
	int _length;
	std::string _type;
	std::string _name;
	std::string _fileName;
};

class BaseHandler : public HTTPRequestHandler
{
public:
	BaseHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");

		std::ostream& ostr = response.send();
		ifstream fin("index.html", ios::binary);
		string content((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

		ostr << content;
	}
};


class ListStoresHandler : public HTTPRequestHandler
{
public:
	ListStoresHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("application/json");

		std::ostream& ostr = response.send();

		vector<Database::Store> stores;
		if (!Database::Store::LoadAll(stores))
		{
			ostr << "{\"error\": \"Cannot load list of stores\"}";
			return;
		}

		json response_json;
		response_json["action"] = "ListStores";
		response_json["stores"] = json::array();

		for (auto it = stores.begin(); it != stores.end(); it++)
		{
			json store_data;
			string data;
			if (!IpfsAdapter::Load(it->m_data_hash, data))
			{
				continue;
			}
			store_data = json::parse(data);
			store_data["id"] = BinToHex(it->m_id);
			response_json["stores"].push_back(store_data);
		}

		ostr << response_json.dump();
	}
};


class ListItemsHandler : public HTTPRequestHandler
{
public:
	ListItemsHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("application/json");

		MyPartHandler partHandler;
		HTMLForm parameters(request, request.stream(), partHandler);

		std::ostream& ostr = response.send();

		if (parameters.has("store_id") == false)
		{
			ostr << "{\"error\": \"Required parameter missing: store_id\"}";
			return;
		}
		string store_id = HexToBin(parameters["store_id"]);
		Database::Store store;
		if (!Database::Store::Load(store_id, store))
		{
			ostr << "{\"error\": \"Store not found\"}";
			return;
		}

		vector<Database::Item> items;
		if (!Database::Item::LoadAll(store_id, items))
		{
			ostr << "{\"error\": \"Cannot load list of items\"}";
			return;
		}

		json json_response;
		json_response["action"] = "ListItems";

		string store_data;
		if (IpfsAdapter::Load(store.m_data_hash, store_data))
		{
			json_response["store"] = json::parse(store_data);
			json_response["store"]["id"] = BinToHex(store.m_id);
		}

		json_response["items"] = json::array();
		for (auto it = items.begin(); it != items.end(); it++)
		{
			json item_data;
			string data;
			if (!IpfsAdapter::Load(it->m_data_hash, data))
			{
				continue;
			}
			item_data = json::parse(data);
			item_data["id"] = BinToHex(it->m_id);
			json_response["items"].push_back(item_data);
		}

		ostr << json_response.dump();
	}
};


class ListCommentsHandler : public HTTPRequestHandler
{
public:
	ListCommentsHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("application/json");

		MyPartHandler partHandler;
		HTMLForm parameters(request, request.stream(), partHandler);

		std::ostream& ostr = response.send();

		if (parameters.has("item_id") == false)
		{
			ostr << "{\"error\": \"Required parameter missing: item_id\"}";
			return;
		}
		string item_id = HexToBin(parameters["item_id"]);
		Database::Item item;
		if (!Database::Item::Load(item_id, item))
		{
			ostr << "{\"error\": \"Item not found\"}";
			return;
		}

		vector<Database::Comment> comments;
		if (!Database::Comment::LoadAll(item_id, comments))
		{
			ostr << "{\"error\": \"Cannot load list of comments\"}";
			return;
		}

		json json_response;
		json_response["action"] = "ListComments";

		string item_data;
		if (IpfsAdapter::Load(item.m_data_hash, item_data))
		{
			json_response["item"] = json::parse(item_data);
			json_response["item"]["id"] = BinToHex(item.m_id);
		}

		json_response["comments"] = json::array();
		for (auto it = comments.begin(); it != comments.end(); it++)
		{
			json comment_data;
			string data;
			if (!IpfsAdapter::Load(it->m_data_hash, data))
			{
				cout << '!';
				continue;
			}
			comment_data = json::parse(data);
			comment_data["id"] = BinToHex(it->m_id);
			json_response["comments"].push_back(comment_data);
		}

		ostr << json_response.dump();
		return;
	}
};


class AddCommentHandler : public HTTPRequestHandler
{
public:
	AddCommentHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("application/json");

		MyPartHandler partHandler;
		HTMLForm parameters(request, request.stream(), partHandler);

		std::ostream& ostr = response.send();

		if (parameters.has("item_id") == false)
		{
			ostr << "{\"error\": \"Required parameter missing: item_id\"}";
			return;
		}
		string item_id = HexToBin(parameters["item_id"]);

		if (parameters.has("signature") == false)
		{
			ostr << "{\"error\": \"Required parameter missing: signature\"}";
			return;
		}
		string signature = HexToBin(parameters["signature"]);

		if (parameters.has("comment_data") == false)
		{
			ostr << "{\"error\": \"Required parameter missing: comment_data\"}";
			return;
		}
		string comment_data = parameters["comment_data"];

		string comment_id = item_id;
		if (Database::Comment::CheckExists(comment_id))
		{
			ostr << "{\"error\": \"You have already created a comment\"}";
			return;
		}
		string comment_item_id = item_id;
		string comment_mt_root = "29837569183465864871254";
		string comment_proof = "1956234756109823680498723";
		string comment_data_hash = Hash(comment_data);
		Database::Comment comment(comment_id, comment_item_id, comment_mt_root, comment_proof, comment_data_hash);
		comment.Save();
		IpfsAdapter::Save(comment_data_hash, comment_data);

		ostr << "{\"action\": \"AddComment\", \"result\": \"processing\"}";
	}
};


class LoadItemHandler : public HTTPRequestHandler
{
public:
	LoadItemHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("application/json");

		MyPartHandler partHandler;
		HTMLForm parameters(request, request.stream(), partHandler);

		std::ostream& ostr = response.send();

		if (parameters.has("item_id") == false)
		{
			ostr << "{\"error\": \"Required parameter missing: item_id\"}";
			return;
		}
		string item_id = HexToBin(parameters["item_id"]);
		Database::Item item;
		if (!Database::Item::Load(item_id, item))
		{
			ostr << "{\"error\": \"Item not found\"}";
			return;
		}

		json json_response;
		json_response["action"] = "LoadItem";

		string item_data;
		if (IpfsAdapter::Load(item.m_data_hash, item_data))
		{
			json_response["item"] = json::parse(item_data);
			json_response["item"]["id"] = BinToHex(item.m_id);
		}
		
		ostr << json_response.dump();
		return;
	}
};


class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	RequestHandlerFactory(const std::string& format) :
		_format(format)
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		if (request.getURI() == "/")
			return new BaseHandler();
		else if (request.getURI().length() >= 11 && request.getURI().substr(0, 11) == "/ListStores")
			return new ListStoresHandler();
		else if (request.getURI().length() >= 10 && request.getURI().substr(0, 10) == "/ListItems")
			return new ListItemsHandler();
		else if (request.getURI().length() >= 13 && request.getURI().substr(0, 13) == "/ListComments")
			return new ListCommentsHandler();
		else if (request.getURI().length() >= 11 && request.getURI().substr(0, 11) == "/AddComment")
			return new AddCommentHandler();
		else if (request.getURI().length() >= 9 && request.getURI().substr(0, 11) == "/LoadItem")
			return new LoadItemHandler();
		else
			return 0;
	}

private:
	std::string _format;
};


class HTTPSTimeServer : public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the HTTPTimeServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (HTTPTimeServer.properties),
	/// copy the file to the directory where the HTTPTimeServer executable
	/// resides. If you start the debug version of the HTTPTimeServer
	/// (HTTPTimeServerd[.exe]), you must also create a copy of the configuration
	/// file named HTTPTimeServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9443) and the format of the date/time string sent back to the client.
	///
	/// To test the TimeServer you can use any web browser (https://localhost:9443/).
{
public:
	HTTPSTimeServer() : _helpRequested(false)
	{
		Poco::Net::initializeSSL();
	}

	~HTTPSTimeServer()
	{
		Poco::Net::uninitializeSSL();
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}

	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A web server that serves the current date and time.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			// get parameters from configuration file
			unsigned short port = (unsigned short)config().getInt("HTTPSTimeServer.port", 9443);
			std::string format(config().getString("HTTPSTimeServer.format", DateTimeFormat::SORTABLE_FORMAT));

			// set-up a server socket
			SecureServerSocket svs(port);
			// set-up a HTTPServer instance
			HTTPServer srv(new RequestHandlerFactory(format), svs, new HTTPServerParams);
			// start the HTTPServer
			srv.start();
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the HTTPServer
			srv.stop();
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
};

thread web_api_thread;

void StartWebApi_internal(int argc, char** argv)
{
	HTTPSTimeServer app;
	app.run(argc, argv);
}


void StartWebApi(int argc, char** argv)
{
	web_api_thread = thread(StartWebApi_internal, argc, argv);
}