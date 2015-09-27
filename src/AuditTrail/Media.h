#pragma once

#include <string>

namespace AuditTrail
{

enum class MediaType
{
    CD,
    DVD
};

static std::string mediaTypeToString(MediaType mediaType)
{
    switch (mediaType)
    {
    case MediaType::CD:     return "CD";
    case MediaType::DVD:    return "DVD";
    default: throw std::logic_error("Unable to convert media type " + std::to_string(static_cast<int>(mediaType)) + " to string");
    }
}

struct Media
{
    Media(MediaType type) : mediaType(type) {}

    MediaType mediaType;
    std::string mediaLabel;

    std::string mediaId() const
    {
        std::string id = mediaTypeToString(mediaType);
        if (!mediaLabel.empty())
            id += " " + mediaLabel;
        return id;
    }
};

}