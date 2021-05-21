using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Chat : MonoBehaviour
{
    public GameObject chatWindow;
    private Text chatText;
    private void Awake()
    {
        chatText = chatWindow.transform.GetChild(0).GetComponent<Text>();
    }
    public void OpenChat()
    {
        chatWindow.SetActive(true);
    }

    public void CloseChat()
    {
        chatWindow.SetActive(false);
    }

    public void SetText(string name)
    {
        NpcInfo npcInfo = GameInfo.Instance.npcInfos[name];
        string conversationKey = $"{npcInfo.npcName}_{npcInfo.conversationIndex}";
        Debug.Log(conversationKey);
        Conversation conversation = GameInfo.Instance.conversationList[conversationKey];


        chatText.text = conversation.talk;
    }
   
}
