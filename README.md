## 🏗️ System Architecture & Workflow

HackBridge operates on a continuous feedback loop between the AI client and the target Linux environment. Below is the architectural flow illustrating how natural language is translated into executed offensive security commands.

```mermaid
graph TD
    %% Custom Styling for Final Review %%
    classDef ai fill:#2B3A67,stroke:#fff,stroke-width:2px,color:#fff,rx:5px,ry:5px;
    classDef bridge fill:#496A81,stroke:#fff,stroke-width:2px,color:#fff,rx:5px,ry:5px;
    classDef api fill:#66999B,stroke:#fff,stroke-width:2px,color:#fff,rx:5px,ry:5px;
    classDef terminal fill:#1D1E18,stroke:#B3C680,stroke-width:2px,color:#B3C680;
    classDef tool fill:#B3C680,stroke:#1D1E18,stroke-width:2px,color:#1D1E18;

    A[🤖 AI Client<br/>Claude Desktop / 5ire / Gemini]:::ai
    
    subgraph Client/Middle Tier
        B[🌉 MCP Bridge<br/>mcp_server.py]:::bridge
    end

    subgraph Linux / Kali Target Environment
        C[🌐 Flask API Server<br/>kali_server.py]:::api
        D>💻 Linux Terminal]:::terminal
        
        E((Nmap / Recon)):::tool
        F((SQLMap / Web)):::tool
        G((Custom Binaries)):::tool
    end

    %% Execution Flow (Downwards) %%
    A -- "1. Standard MCP Request<br/>(e.g., call_tool: nmap_scan)" --> B
    B -- "2. HTTP POST Request<br/>(Port 5000)" --> C
    C -- "3. Subprocess Execution<br/>(Timeout Managed)" --> D
    
    D -- "4. Execute Payload" --> E
    D -- "4. Execute Payload" --> F
    D -- "4. Execute Payload" --> G
    
    %% Return Flow (Upwards) %%
    E -. "5. Raw stdout/stderr" .-> D
    F -. "5. Raw stdout/stderr" .-> D
    G -. "5. Raw stdout/stderr" .-> D
    
    D -. "6. Captured Output" .-> C
    C -. "7. Parsed JSON Response" .-> B
    B -. "8. Structured Context" .-> A
