#pragma once
template<int N>
inline void reverse(byte* a, byte *b) {
	*a = *b;
	reverse<N - 1>(a + 1, b - 1);
}
template<>
inline void reverse<1>(byte* a, byte *b) {
	*a = *b;
}
class MemoryStream
{
	
public:
	string data;
	int offset;
	MemoryStream():offset(0),data()
	{
	}
	MemoryStream(const MemoryStream& right):data(),offset(right.offset) {
		data.append(right.data);
	}
	MemoryStream& operator = (const MemoryStream& right) {
		if (this  != &right) {
			data = right.data;
			offset = right.offset;
		}
		return *this;
	}
	MemoryStream(MemoryStream&& right):data(move(right.data)),offset(right.offset) {
		
	}
	MemoryStream& operator = (MemoryStream&& right) {
		if (this != &right) {
			data = move(right.data);
			offset = right.offset;
		}
		return *this;
	}
	MemoryStream(string&& right) :data(move(right)), offset(0) {

	}
	MemoryStream& operator = (string&& right) {
		data = move(right);
		offset = 0;
		return *this;
	}
	MemoryStream operator + (MemoryStream&& right) {
		return MemoryStream(data+right.data);
	}
	MemoryStream& operator >>=(const int& i)
	{
		offset += i;
		return *this;
	}
	MemoryStream& operator <<=(const int& i)
	{
		offset -= i;
		return *this;
	}
	MemoryStream& operator <<(const string& i)
	{
		data.append(i);
		return *this;
	}
	MemoryStream& operator <<(const MemoryStream& i)
	{
		data.append(i.data);
		return *this;
	}
	operator const char*() {
		return data.data() + offset;
	}
	byte& operator [](int i)
	{
		byte* bytes = (byte*)data.data() + offset;
		return bytes[i];
	}
	template<int N,class T>
	MemoryStream& readB(T& out) {
		reverse<N>((byte*)&out, (byte*)data.data() + offset + N - 1);
		offset += N;
		return *this;
	}
	template<int N, class T>
	T readB() {
		T out;
		reverse<N>((byte*)&out, (byte*)data.data() + offset + N - 1);
		offset += N;
		return out;
	}
#define readByte readB<1,byte>
#define readUInt16B readB<2,unsigned short>
#define readUInt24B readB<3,unsigned int>
#define readUInt32B readB<4,unsigned int>
#define readDoubleB readB<8,double>
#define read1 readB<1>
#define read2B readB<2>
#define read3B readB<3>
#define read4B readB<4>
#define read8B readB<8>
	template<class T>
	MemoryStream& readNormal(T& out) {
		out = *(T*)(data.data() + offset);
		offset += sizeof(T);
		return *this;
	}
	template<class T>
	T readNormal() {
		T out;
		out = *(T*)(data.data() + offset);
		offset += sizeof(T);
		return out;
	}
	string readString(size_t len = string::npos) {
		string result = data.substr(offset, len);
		offset += result.length();
		return result;
	}
	void clear() {
		data.clear();
		offset = 0;
	}
	int size() {
		return data.length();
	}
	int length() {
		return data.length() - offset;
	}
	~MemoryStream()
	{
	}
	void consoleHex() {
		string output(3 * size() + 1, '\0');
		char* hex_buf = (char *)output.c_str();
		bool hasOffset = false;
		for (int i = 0; i < size(); ++i)
		{
			unsigned char c = data[i];
			unsigned int nIntVal = c;
			if (!hasOffset) {
				if (offset == i) {
					hasOffset = true;
					hex_buf[i * 3] = '#';
				}
			}
			sprintf(hex_buf + i * 3 + (hasOffset ? 1 : 0), "%02x,", nIntVal);
		}
		val::global("console").call<void>("log", output);
	}
};

