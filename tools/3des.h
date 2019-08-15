#ifndef _DES_H_
#define _DES_H_
#include <string>
#include <sstream>
using namespace std;

//Ϊ����߳���Ч�ʣ���������λ�������ܶ����ںꡣ

//��ȡ��������ָ��λ.
#define GET_BIT(p_array, bit_index)  \
((p_array[(bit_index) >> 3] >> (7 - ((bit_index) & 0x07))) & 0x01)

//���û�������ָ��λ.
#define SET_BIT(p_array, bit_index, bit_val) \
if (1 == (bit_val)) \
{\
p_array[(bit_index) >> 3] |= 0x01 << (7 - ((bit_index) & 0x07));\
}\
else\
{\
p_array[(bit_index) >> 3] &= ~(0x01 << (7 - ((bit_index) & 0x07)));\
}

//�ӽ��ܱ�ʶ����������ʶ�漰���Ա�Ķ�ȡλ��,
//���뱣֤DES_ENCRYPT = 0 DES_DECRYPT = 1
typedef enum {
	DES_ENCRYPT = 0, DES_DECRYPT = 1
} DES_MODE;

//key
typedef enum {
	//1keyģʽ ��ͬ��des
	KEY_16,
	//2keyģʽ key1 �� key3
	KEY_32,
	//3keyģʽ
	KEY_48,
	KEY_ERROR
} KEYMODE;

class Des {
private:
	std::string deskey;
private:
	//�ַ��������
	void print_binary(char * tip, unsigned char * buff, unsigned char bits);
	void print_hex(char * tip, unsigned char * buff, unsigned char bytes);
	///////////////////////////////////////////////////////////////
	//  �� �� �� : des
	//  �������� : DES�ӽ���
	//  ������� : ���ݱ�׼��DES�����㷨�������64λ��Կ��64λ���Ľ��м�/����
	//              ������/���ܽ���洢��p_output��
	//  ʱ    �� : 2006��9��2��
	//  �� �� ֵ :
	//  ����˵�� :  const char * p_data     ����, ����ʱ��������, ����ʱ��������, 64λ(8�ֽ�)
	//              const char * p_key      ����, ��Կ, 64λ(8�ֽ�)
	//              char * p_output         ���, ����ʱ�������, ����ʱ��������, 64λ(8�ֽ�)
	//              unsigned char mode              0 ����  1 ����
	///////////////////////////////////////////////////////////////
	void des(const char * p_data, const char * p_key, const char * p_output,
		DES_MODE mode);
	//�������С��ͬ���ڴ����������
	//��������浽��һ���ڴ�
	//unsigned char * p_buf_1       �ڴ���1
	//const unsigned char * p_buf_2 �ڴ���2
	//unsigned char bytes           �ڴ�����С(��λ���ֽ�)
	void Xor(unsigned char * p_buf_1, const unsigned char * p_buf_2,
		unsigned char bytes);
	//���������ӵ�bit_startλ����bit_end����ѭ������
	//offsetֻ����1��2
	void move_left(unsigned char * p_input, unsigned char bit_start,
		unsigned char bit_end, unsigned char offset);
	//���������ӵ�bit_startλ����bit_end����ѭ������
	//offsetֻ����1��2
	void move_right(unsigned char * p_input, unsigned char bit_start,
		unsigned char bit_end, unsigned char offset);
	//��������λ
	//offset����0ʱ����
	//offsetС��0ʱ����
	void move_bits(unsigned char * p_input, unsigned char bit_start,
		unsigned char bit_end, char offset);
	//ͨ���û�����, bits <= 64
	//p_input��p_output����ָ��ͬһ����ַ�������û������
	void Permutation(unsigned char * p_input, unsigned char * p_output,
		const unsigned char * Table, unsigned char bits);
	//��ȡ��bit_sΪ��ʼ�ĵ�1, 6 λ�����
	unsigned char S_GetLine(unsigned char * p_data_ext, unsigned char bit_s);
	//��ȡ��bit_sΪ��ʼ�ĵ�2,3,4,5λ�����
	unsigned char S_GetRow(unsigned char * p_data_ext, unsigned char bit_s);

	/************************************************************************
	*���ܣ��ַ���ת��Ϊ16���Ʊ�ʾ��ÿ��λΪһ��
	************************************************************************/
	unsigned char* str_to_hex(const std::string& str);
	/************************************************************************
	*���ܣ����3DES�����key
	************************************************************************/
	unsigned char *getKey1(std::string key);
	unsigned char *getKey2(std::string key);
	unsigned char *getKey3(std::string key);
	/************************************************************************
	*���ܣ��������
	************************************************************************/
	void clearBuff(char *buff, int length);
	/*����
	DataByte
	[in]��������ݳ���,���ֽ�Ϊ��λ
	*/
	std::string base64Encode(const unsigned char* Data, int DataByte);
	/*����
	DataByte
	[in]��������ݳ���,���ֽ�Ϊ��λ
	OutByte
	[out]��������ݳ���,���ֽ�Ϊ��λ,�벻Ҫͨ������ֵ����
	������ݵĳ���
	*/
	std::string base64Decode(const char* Data, int DataByte, int& OutByte);

	//���ܣ�URL�ַ�ת��
	unsigned char ToHex(unsigned char x);
	unsigned char FromHex(unsigned char x);
	//���ܣ�URL-ENCODE����
	std::string UrlEncode(const std::string& str);
	//���ܣ�URL-DECODE����
	std::string UrlDecode(const std::string& str);

	//�滻�ַ���
	std::string replaceAllSubStr(string &str, string &oldStr, string &newStr);

	//���key�ĳ��ȣ����ؼ���ģʽ
	KEYMODE checkKey();
	KEYMODE checkKey(std::string key);

public:
	Des() {
	}
	;
	Des(std::string key) {
		deskey = key;
	}
	;

	~Des() {
		deskey = nullptr;
	}
	;
	/************************************************************************
	*���ܣ�3DES-ECB����
	************************************************************************/
	std::string desEncrypt(std::string str);

	/************************************************************************
	*���ܣ�3DES-ECB����
	************************************************************************/
	std::string desDecrypt(std::string str);

};
#endif //#ifndef _DES_H_