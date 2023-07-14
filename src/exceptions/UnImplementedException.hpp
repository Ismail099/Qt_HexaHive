//
// Created by 3301-byte on 02.02.2023.
//

#ifndef UTORRENT_UNIMPLEMENTEDEXCEPTION_HPP
#define UTORRENT_UNIMPLEMENTEDEXCEPTION_HPP


#include <QException>

/*!
 * @brief UnImplementedException class
 */
class UnImplementedException : public QException {

	public:

		explicit UnImplementedException(QString const& message);
		~UnImplementedException() override;

		void raise() const override;

		UnImplementedException *clone();

		QString message() const;

	private:
		QString m_Message;
};


#endif //UTORRENT_UNIMPLEMENTEDEXCEPTION_HPP
