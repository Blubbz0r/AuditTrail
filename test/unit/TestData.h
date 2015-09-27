#pragma once

#include <string>

namespace Tests
{

namespace DICOM
{

    const std::string ArbitraryAETitle = "ARCHIVE";
    const std::string ArbitraryAETitle2 = "ARCHIVE2";
    const std::string ArbitraryAETitle3 = "VIEWER";

    const std::string ArbitraryStudyInstanceUID = "1.2.840.10008.5.1.4.1.1.2";
    const std::string ArbitrarySOPClassUID = "1.2.840.10008.5.1.4.1.1.2.123";
    const int ArbitraryNumberOfInstances = 1000;

    const std::string ArbitraryPatientID = "ID001";
    const std::string ArbitraryPatientName = "Sandra Newman";

}

namespace Machine
{

    const std::string ArbitraryMachineName = "CT001";

    const std::string ArbitraryIPAddress = "127.0.0.1";

}

namespace Process
{

    const std::string ArbitraryProcessID = "123";

    const std::string ArbitraryProcessName = "Archive";

}

namespace User
{

    const std::string ArbitraryUserID = "john.doe@gmail.com";

    const std::string ArbitraryAlternativeUserID = "MS\\doe";

    const std::string ArbitraryUserName = "John Doe";

    const std::string ArbitraryTelephoneNumber = "123456789";

}

}