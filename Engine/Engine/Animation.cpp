#include "Animation.h"

Animation::Animation(unsigned int updateRate)
{
	this->updateRateCount = 1;
	this->updateRate = updateRate;
}

void Animation::update()
{
	if (updateRateCount >= updateRate / MS_PER_UPDATE)
	{
		bool behindLastKeyframe = true;

		for (unsigned int i = 0; i < this->Keyframes.size(); i++)
		{
			if (this->Keyframes.at(i)->getTimeStamp() == this->getTime())
			{
				this->Keyframes.at(i)->activateKeyframe();
			}
			if (this->Keyframes.at(i)->getTimeStamp() >= this->getTime())
				behindLastKeyframe = false;
		}

		bool noAnimationRunning = true;
		for (unsigned int i = 0; i < this->subAnimations.size(); i++)
		{
			if (this->subAnimations.atIndex(i)->isRunning())
			{
				for (unsigned int j = 0; j < this->objects.size(); j++)
				{
					this->subAnimations.atIndex(i)->update(this->objects.at(j));
				}

				this->subAnimations.atIndex(i)->increaseTimeCounter();
				noAnimationRunning = false;
			}
		}

		if (behindLastKeyframe && noAnimationRunning)
			if (this->loop)
			{
				this->timeCount = 0;
				this->update();
			}
			else
				this->running = false;
		else
			this->timeCount++;
		updateRateCount = 1;
	}
	else
	{
		updateRateCount++;
	}
}

void Animation::update(AnimationObject * object)
{
	this->update();
}

void Animation::addSubAnimation(string name, SubAnimation* animation, unsigned int time)
{
	animation->setUpdateRate(this->updateRate);
	this->subAnimations.push(name, animation);
	this->addKeyframe(name, ANISTART, time);
}

void Animation::addKeyframe(string name, KeyframeAction action, unsigned int time)
{
	Keyframe* frame = nullptr;
	
	/*if (this->Keyframes.end() != std::find(this->Keyframes.begin(), this->Keyframes.find(time))
	{
		frame = this->Keyframes.at(time);
	}
	else
	{
		frame = new Keyframe(time);
		this->Keyframes.emplace(time, frame);
	}*/

	switch (action)
	{
		case ANISTART:
			frame->addAction([&, name]() { this->subAnimations.at(name)->start(); });
			break;
		case ANIPAUSE:
			frame->addAction([&, name]() {this->subAnimations.at(name)->pause(); });
			break;
		case ANIRESTART:
			frame->addAction([&, name]() {this->subAnimations.at(name)->restart(); });
			break;
		case ANIRESUME:
			frame->addAction([&, name]() {this->subAnimations.at(name)->resume(); });
			break;
	}
}

void Animation::removeKeyframe(unsigned int time)
{
	for (unsigned int i = 0; i < this->Keyframes.size(); i++)
	{
		if (time == this->Keyframes.at(i)->getTimeStamp())
		{
			this->Keyframes.erase(this->Keyframes.begin() + i);
			return;
		}
	}
}

void Animation::addObject(AnimationObject* object)
{
	this->objects.push_back(object);
}

void Animation::removeObject(AnimationObject* object)
{
	for (unsigned int i = 0; i < this->objects.size(); i++)
	{
		if (this->objects.at(i) == object)
		{
			this->objects.erase(objects.begin() + i);
			return;
		}
	}
}

