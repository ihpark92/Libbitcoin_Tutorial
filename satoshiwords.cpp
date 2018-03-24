#include <bitcoin/bitcoin.hpp>
using namespace bc;

int main()
{
    // Extracting Satoshi's words from genesis block.
    const auto block = bc::chain::block::genesis_mainnet();
    const auto& coinbase = block.transactions().front();
    const auto& input = coinbase.inputs().front();
	BITCOIN_ASSERT_MSG(input.script().size() > 2u, "unexpected genesis");

	const auto headline = input.script()[2].data();
	std::string message(headline.begin(), headline.end());
    bc::cout << message << std::endl;

    return EXIT_SUCCESS;
}
