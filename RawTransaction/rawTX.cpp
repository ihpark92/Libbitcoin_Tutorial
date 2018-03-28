#include <bitcoin/bitcoin.hpp>
#include "HD_Wallet_Testnet.cpp"
#include <string.h>
 
using namespace bc;
using namespace bc::wallet;
using namespace bc::machine;
using namespace bc::chain;

std::string getInput2()
{
     std::string input; 
     getline(cin, input);
     return input;
}

int main() 
{
     //import Wallet
     std::cout << "Import Wallet Via Mnemonic: " << std::endl;
     std::string Mnemonic1 = getInput2();
     HD_Wallet wallet1(split(Mnemonic1));
     std::cout << "\nChild Index To Spend From: " << std::endl;
     int child = atoi(getInput2().c_str());
     data_chunk pubkey1 = to_chunk(wallet1.childPublicKey(child).point());

	//Make Output//
	std::cout << "\nEnter Destination Address: " << std::endl;
	std::string Destination = getInput2(); //"mi9dcMmV2FdmfNJkgpCYHvoc6iPqWSnGo8";
	payment_address destinationAddy(Destination);
	script outputScript = script().to_pay_key_hash_pattern(destinationAddy.hash());

	std::cout << "\nEnter Amount(BTC) To Send: " << std::endl;
	std::string BTC = getInput2();
	uint64_t Satoshis;
	decode_base10(Satoshis, BTC, 8);
	output output1(Satoshis, outputScript);
	std::cout << "\nAmount: " << encode_base10(output1.value(), 8) << "BTC : Output Script: " << output1.script().to_string(0) << "\n" << std::endl;

	//Get UTXO
	std::cout << "\nEnter UTXO Hash To Spend: " << std::endl;
	std::string hashString = getInput2(); //"87fb96a7105e5cf840b5b9a7af3bc30641e49871893ffb42ac17ec3913ca3b30";
	std::cout << "\nEnter Output Index: " << std::endl;
	std::string index = getInput2();
	uint32_t index1 = atoi(index.c_str());
	hash_digest utxoHash;  
	decode_hash(utxoHash, hashString);
	output_point utxo(utxoHash, index1);

	//Previous Locking Script
	script lockingScript = script().to_pay_key_hash_pattern(bitcoin_short_hash(pubkey1));

	//make Input
	input input1 = input();
	input1.set_previous_output(utxo);
	input1.set_sequence(0xffffffff);

	std::cout << "\nPrevious Locking Script: " << std::endl;
	std::cout << lockingScript.to_string(0) << "\n" << std::endl;

	//build TX
	transaction tx = transaction();
	tx.inputs().push_back(input1);
	tx.outputs().push_back(output1);

	//Endorse TX
	endorsement sig; 
	if(lockingScript.create_endorsement(sig, wallet1.childPrivateKey(1).secret(), lockingScript, tx, 0u, all))
	{
		std::cout << "Signature: " << std::endl;
		std::cout << encode_base16(sig) << "\n" << std::endl; 
	}

	//make Sig Script
	operation::list sigScript; 
	sigScript.push_back(operation(sig));
	sigScript.push_back(operation(pubkey1));
	script unlockingScript(sigScript);

	//Make Signed TX
	tx.inputs()[0].set_script(unlockingScript);
	std::cout << "Raw Transaction: " << std::endl;
	std::cout << encode_base16(tx.to_data()) << std::endl;
}
