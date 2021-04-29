#pragma once

#include "Individual.h"
#include "string_util.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/document.h"
#include <map>


class ClientsManager
{
private:
	ClientsManager() {};
	ClientsManager(const ClientsManager&) {};
	const ClientsManager& operator=(const ClientsManager&) {};
	~ClientsManager() {};

private:
	vector<Individual*> all_clients;
	vector<Individual*> invited_clients;
	vector<Individual*> all_participants;

	map<string, Individual*> individualMap;
	map<string, Individual*> invitationMap;
	map<string, Individual*> participantsMap;
	static ClientsManager* instance_;

public:
	static ClientsManager* getInstance();

public:
	vector<Individual*> getClients(string rsp);
	vector<Individual*> getAllClients();
	map<string, Individual*> getIndividualMap();
	map<string, Individual*>  getInvitationMap();
	vector<Individual*> getInvitedClients();
	vector<Individual*> getParticipants(UINT *uids, UINT count);
	vector<Individual*> getAllParticipants();

	void addParticipant(Individual* individual);
	void removeParticipant(Individual* individual);
	void addInvitedClient(Individual* client);
	void clearInvitedClients();
	void updateInvitedMap(string user_id, Individual* client);
	bool isClientInvited(Individual* client);
	void removeClient(Individual* client);
	Individual* updateParticipant(string resp);
};