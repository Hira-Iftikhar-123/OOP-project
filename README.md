The Blood Bank Management System (BBS) is a comprehensive software solution developed to efficiently manage various operations within a blood bank. Here's a brief overview of its functionality and core components:

Overview

The BBS is designed to handle the complexities of blood bank operations, including blood donation, inventory management, and compatibility checks for blood transfusions. Using object-oriented programming, the system is modular and easy to maintain.

Key Components

1. User, Admin, Citizen, Donor, Receiver Classes:
    - These classes represent different users of the system, each with specific roles and responsibilities. For example, the `Donor` class manages donor information and donation records, while the `Receiver` class handles blood requests.

2. Blood Class:
    - This class is central to the system, managing the blood inventory. It tracks the number of blood packets for each blood type and handles updates to the inventory when blood is donated or transfused.

3. Compatibility Check:
    - A function called `showCompatibility` helps determine which blood types are compatible with a given blood group, aiding in the blood transfusion process.

4. File Handling:
    - The system uses file I/O to store and retrieve data, ensuring that blood inventory and user records are maintained persistently.

5. Menu Class:
    - This class provides the user interface, allowing users to navigate through the system and perform various operations such as viewing blood availability, registering as donors, and making blood requests.

6. Helper Classes (e.g., Date, Transport):
    - These support classes handle specific tasks, like managing donation dates and organizing blood transport logistics.

 Main Functionalities

- Blood Donation:
    - Donors can register and donate blood, with the system updating the inventory to reflect new donations.

- Blood Request and Transfusion:
    - The system allows receivers to request blood, checks compatibility, and processes the request if the appropriate blood type is available.

- Inventory Management:
    - The BBS keeps an updated inventory of blood packets, providing real-time information on the availability of different blood types.

- User Management:
    - The system supports various user roles, each with access to specific functionalities tailored to their needs, such as managing inventory for admins or registering donations for donors.
