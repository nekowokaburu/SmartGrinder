#include "communicator.hpp"

Communicator::Communicator() : receivedMessage_(""), receivedCommand_{Command::None}
{
    Serial.begin(57600);
    while (!Serial)
        ;

    // This helps to see if the device crashes or resets at some point.
    // Serial.println("Communicator initialized!");
}

enum Communicator::Command Communicator::Command() noexcept
{
    auto const receivedCommandTmp = receivedCommand_;
    receivedCommand_ = Command::None;
    return receivedCommandTmp;
}

void Communicator::Update() noexcept
{
    // Reset the received message which is only valid for one update cycle and contains the value as well
    receivedMessage_ = "";

    // Might be better to only do this once and rely on outer update loop?
    // Actually this should also only run if the Command and value were retrieved. Can be protected here or in VBM
    // class logic (currently the case).
    while (Serial.available())
    {
        receivedMessage_ += (char)Serial.read();
    }

    if (receivedMessage_.length())
    {  // if string is not empty do the following
        LOG_COMM(String("Received Message: ") + receivedMessage_)

        auto receivedMessageLower(receivedMessage_);
        receivedMessageLower.toLowerCase();
        receivedMessageLower.trim();

        if (receivedMessageLower.startsWith(String("updateapp")))
        {
            receivedCommand_ = Command::UpdateApp;
        }
        else if (receivedMessageLower.startsWith(String("short:")))
        {
            receivedCommand_ = Command::ShortShotTime;
        }
        else if (receivedMessageLower.startsWith(String("long:")))
        {
            receivedCommand_ = Command::LongShotTime;
        }
    }
}

void Communicator::SendMessageOnce(String Message) const noexcept
{
    LOG_COMM(String("Sending message: ") + Message)
    Serial.print(Message);
}

// void Communicator::SendMessageOnce(String Message, double Value) const noexcept
// {
//     SendMessageOnce(String(">") + Message + ":" + Value);
// }
