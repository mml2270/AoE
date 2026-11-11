
flowchart TD
    P1["Player 1 IR Break-Beam Sensor"]
    P2["Player 2 IR Break-Beam Sensor"]
    PIEZO["Piezoelectric Impact Sensor"]

    P1 -->|"Hand entry signal - D2"| A
    P2 -->|"Hand entry signal - D3"| A
    PIEZO -->|"Impact signal - A0"| A

    A["Shared Arduino"]

    A --> TIME["Record Event Timestamps"]
    TIME --> CHECK{"Recent hand entry and impact?"}

    CHECK -->|"No"| WAIT["Ignore or flag event"]
    CHECK -->|"Yes"| COMPARE["Compare Player 1 and 2 Timing"]

    COMPARE --> RESULT{"Clear timing difference?"}

    RESULT -->|"Yes"| WIN["Report Likely First Player"]
    RESULT -->|"No"| TIE["Report Ambiguous Event"]

    classDef sensor fill:#dbeafe,stroke:#2563eb,color:#1e3a8a
    classDef process fill:#dcfce7,stroke:#16a34a,color:#14532d
    classDef result fill:#fef3c7,stroke:#d97706,color:#78350f

    class P1,P2,PIEZO sensor
    class A,TIME,CHECK,COMPARE,RESULT process
    class WAIT,WIN,TIE result
