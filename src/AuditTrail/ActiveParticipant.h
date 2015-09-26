#pragma once

#include "CodedValueType.h"

#include <string>
#include <vector>

namespace AuditTrail
{

enum class NetworkAccessPointType
{
    None,
    MachineName,
    IPAddress,
    TelephoneNumber,
    EmailAddress,
    URI
};

struct ActiveParticipant
{
    ActiveParticipant(std::string userId, bool userIsRequestor);

    /*!
        If participant is a
            Person:
                - loginName@domain-name
            Process:
                - Identifier of the process, e.g. process ID (has to be documented in DICOM
                  Conformance Statement; see Part15 A.6)
            Data import/export:
                - URI identifying source/destination
                - Email address of the form "mailto:user@address"
                - Description of media type (e.g. DVD) together with description of its identifying
                  label
                - Description of media type (e.g. paper, film) together with description of the
                  location of the media creator (e.g. the printer)
    */
    std::string userId;

    /*!
        Indicator that user is or is not the requestor or initiator.
    */
    bool userIsRequestor;

    /*!
        If participant is a
            Person:
                - Identifier for the person within an enterprise for authentication purposes (e.g.
                  Kerberos username "user@realm"). Use setAlternativeUserId.
            DICOM application:
                - One or more AE titles. Use addAETitle.
    */
    std::string alternativeUserId() const;
    void setAlternativeUserId(std::string alternativeUserId) { m_alternativeUserId = std::move(alternativeUserId); }
    void addAETitle(std::string aeTitle);

    /*!
        If participant is a
            Person:
                - Person's name
            Process:
                - Process name
    */
    std::string userName;

    /*!
        Specification of the role(s) the participant plays when performing the event.
    */
    CodedValueType roleIdCode;

    /*!
        Identifier for the type of network access point.
    */
    NetworkAccessPointType networkAccessPointType;

    /*!
        Identifier for the network access point of the participant (e.g. device id, IP address).
    */
    std::string networkAccessPointId;

private:
    std::string m_alternativeUserId;
    std::vector<std::string> m_aeTitles;
};

}
