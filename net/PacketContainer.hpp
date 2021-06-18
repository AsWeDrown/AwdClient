#pragma once


#include <google/protobuf/message.h>

namespace awd::net {

    /**
     * Хранит содержимое отправленных пакетов (при этом беря НЕ на себя ответственность
     * за управление памятью, занимаемой указателями на пакеты). Используется для
     * временного хранения пакетов в очереди отправленных пакетов на случай, если
     * какой-то пакет нужно будет отправить повторно.
     *
     * Ещё раз: ответственность за высвобождение памяти, занимаемой содержимым оригинального
     * пакета ("delete packet;") лежит на пользователе этого класса. Сам класс никаких активных
     * действий предпринимать в этом плане не будет.
     */
    class PacketContainer {
    private:
        /**
         * Если true, то в случаях, когда будет возникать достаточная уверенность в том,
         * что пакет не был доставлен до цели, будет сконструирован новый пакет, с новыми
         * sequence number и прочими "протоколообразующими" полями, однако с тем же самым
         * содержимым (см. поле packet).
         */
        bool ensureDelivered;

        /**
         * Sequence number, который был у этого пакета при отправке.
         */
        uint32_t sequence;

        /**
         * Указатель на оригинальное содержимое отправленного пакета.
         */
        std::shared_ptr<google::protobuf::Message> packet;

    public:
        PacketContainer(bool ensureDelivered,
                        uint32_t sequence,
                        const std::shared_ptr<google::protobuf::Message>& packet);

        bool shouldEnsureDelivered() const;
        uint32_t getOriginalSequence() const;
        std::shared_ptr<google::protobuf::Message> getOriginalPacket() const;
    };

}

