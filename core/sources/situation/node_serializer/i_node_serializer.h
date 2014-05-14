#ifndef I_NODE_SERIALIZER_H
#define I_NODE_SERIALIZER_H

#include "situation_traits.h"

#include <QByteArray>

namespace situation
{
    class INodeSerializer
    {
    public:
        INodeSerializer();

        bool save(const NodePtr& node, const QString& name) const;
        NodePtr load(const QString& name) const;

        virtual QByteArray toByteArray(const NodePtr& node) const = 0;
        virtual NodePtr fromByteArray(const QByteArray& array) const = 0;
    };
}

#endif // I_NODE_SERIALIZER_H
