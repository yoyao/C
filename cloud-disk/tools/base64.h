//
//  base64 encoding and decoding with C++.
//  Version: 1.01.00
//

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>

class Base64Tool
{
public:
	Base64Tool();
	~Base64Tool();

	static std::string base64_encode(unsigned char const*, unsigned int len);
	static std::string base64_decode(std::string const& s);
	static inline bool is_base64(unsigned char c);

private:

};


#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
