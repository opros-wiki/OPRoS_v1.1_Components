#ifndef __MESSAGE_QUEUE_H_
#define __MESSAGE_QUEUE_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <queue>

class MessageQueueExpiredException : public std::exception
{
public:
	MessageQueueExpiredException(const std::string& message = std::string("Message Queue Expired"))
		: std::exception(message.c_str())
	{}
};

template<typename Message>
class MessageQueue
{
public:
	MessageQueue()
		: clearFlag(false), blockedPopThreadCount(0)
	{}

public:
	void Clear()
	{	
		mutex.lock();
		clearFlag = true;

		condition.notify_all();
		mutex.unlock();

		for (; blockedPopThreadCount != 0;)
		{
			boost::this_thread::yield();	
		}

		mutex.lock();		
		for (; messageQueue.empty() == false;)
		{
			messageQueue.pop();
		}

		clearFlag = false;
		mutex.unlock();		
	}

	void Push(const Message& message)
	{
		boost::mutex::scoped_lock lock(mutex);
		messageQueue.push(message);
		condition.notify_one();
	}

	Message Pop() throw(MessageQueueExpiredException)
	{
		blockedPopThreadCount++;
		boost::mutex::scoped_lock lock(mutex);

		for (; messageQueue.empty() && clearFlag == false;)
		{	
			condition.wait(lock);
		}

		if (clearFlag == true)
		{
			--blockedPopThreadCount;
			throw MessageQueueExpiredException();
		}
		--blockedPopThreadCount;

		Message result = messageQueue.front();
		messageQueue.pop();

		return result;
	}

private:
	volatile bool clearFlag;
	volatile int blockedPopThreadCount;

	boost::mutex mutex;
	boost::condition condition;
	std::queue<Message> messageQueue;
};

#endif //__MESSAGE_QUEUE_H_