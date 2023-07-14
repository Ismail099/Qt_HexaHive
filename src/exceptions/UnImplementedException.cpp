//
// Created by hedar on 02.02.2023.
//

#include "UnImplementedException.hpp"


// Constructor
UnImplementedException::UnImplementedException(const QString &message) : m_Message(message) {
}


// Destructor
UnImplementedException::~UnImplementedException() = default;


// Raise
void UnImplementedException::raise() const {
	throw *this;
}


// Clone
UnImplementedException *UnImplementedException::clone() {
	return new UnImplementedException(*this);
}


// Message
QString UnImplementedException::message() const {
	return m_Message;
}

