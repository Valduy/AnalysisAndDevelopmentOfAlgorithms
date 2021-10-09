#ifndef HAMMURABI_ISERIALIZER_H_
#define HAMMURABI_ISERIALIZER_H_

#include <iostream>

class ISerializable {
public:
	virtual ~ISerializable() {}

	virtual void Serialize(std::ostream& os) = 0;
	virtual void Deserialize(std::istream& is) = 0;	
};

#endif // HAMMURABI_ISERIALIZER_H_