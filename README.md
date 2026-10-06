課題報告：Advanced Task 4-1 HC-05 Master/Slave Communication \& GitHub Collaboration



學生姓名：黃威霖 耿同德



學生學號：113511052 113511101



完成日期：2026-10-08



\---



1\. 實驗目標



學習使用 AT 指令配置 HC-05 藍牙模組，設定 Master（主機）與 Slave（從機）角色，並建立配對連線。



實作雙向無線控制通訊協定（Bidirectional Control Protocol），達成透過 Master 按鈕即時切換 Slave 端 LED 狀態，以及透過 Slave 可變電阻回傳類比值動態調節 Master 端直流馬達轉速。



實踐標準 Git/GitHub 團隊協作工作流程，練習倉庫建立、分支隔離開發（Feature Branching）、Commit 提交、Pull Request 發起與程式碼審查（Code Review）及合併。



2\. 設備與元件



Arduino Uno 開發板 x 2



USB Type-B 傳輸線 x 2



個人電腦（已安裝 Arduino IDE）x 2



麵包版 x 2



HC-05  x 2



L293D x 1



5V DC Motor x 1



可變電阻 x 1



按鈕 x 1



LED x 1



杜邦線 x 若干



電阻 x 若干





3\. 操作說明與成果



硬體接線與燒錄：



Master 端（Student A）：將 HC-05 透過 AT 指令設定為 MASTER 模式。數位接腳 Pin 2 連接按鈕開關；Pin 9（PWM）、Pin 4、Pin 5 連接 L293D 驅動晶片以控制直流馬達；Pin 10（RX）與 Pin 11（TX）透過 SoftwareSerial 連接 HC-05 的 TXD 與 RXD。



Slave 端（Student B）：將 HC-05 透過 AT 指令設定為 SLAVE 模式並記錄 MAC 位址供 Master 綁定。類比接腳 A0 連接可變電阻；Pin 13 連接 LED ，Pin 10（RX）與 Pin 11（TX）透過 SoftwareSerial 連接 HC-05 的 TXD 與 RXD。



通訊協定設計：兩端通訊鮑率皆設定為 9600 bps。Master 偵測按鈕狀態變化並加入 30ms 防彈跳機制，按下時發送1、放開時發送0 控制 Slave LED ，Slave 每 100ms 取樣可變電阻數值，將 0～1023 映射至 0～255（PWM 範圍），並在數值變化量大於 2 時才傳送速度字串，以避免雜訊塞爆傳輸緩衝區。



GitHub 協作流程：Student A 於 GitHub 建立公開倉庫 Bluetooth-Bidirectional-Control 並邀請 Student B 成為 Collaborator。兩位成員分別於獨立分支（如 feature/slave-firmware）進行開發，完成後提交 Commit 並推送至遠端，最後透過 Pull Request 進行 Code Review 並合併至主幹分支 main。



實驗成果：



Master ->  Slave 控制：按下 Master 端按鈕時，Slave 端 LED 立即點亮；放開按鈕時，LED 立即熄滅，狀態切換流暢無可感遲滯。

Slave -> Master 控制：旋轉 Slave 端的可變電阻時，類比數值能即時經由藍牙回傳至 Master 端，Master 端直流馬達轉速隨之平順增減，達成了精確的遠端雙向控制。

通訊穩定性：透過在發送端實施變化量門檻過濾（Deadband Filter），有效抑制了類比訊號的微小漂移雜訊，使藍牙通訊頻寬維持穩定，未發生緩衝區溢位或指令遺失狀況。



操作影片：請參閱同目錄下 DemoVideo/Advanced\_4-1.mp4 之實際操作畫面。



