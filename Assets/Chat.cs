using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Chat : MonoBehaviour
{
    public GameObject chatWindow;
    private Text chatText;
    private Transform buttonPanel;
    [SerializeField] private GameObject[] buttons;
    [SerializeField] private Text[] buttonTexts;
    [SerializeField] private string currentNpcName;

    private void Awake()
    {
        chatText = chatWindow.transform.GetChild(0).GetComponent<Text>();
        buttonPanel = chatWindow.transform.GetChild(1);

        buttons = new GameObject[buttonPanel.childCount];
        buttonTexts = new Text[buttons.Length];

        for (int i = 0; i < buttons.Length; i++)
        {
            buttons[i] = buttonPanel.GetChild(i).gameObject;
            buttonTexts[i] = buttons[i].transform.GetChild(0).GetComponent<Text>();
            buttons[i].SetActive(false);
        }
    }

    private void Start()
    {
        currentNpcName = string.Empty;
        CloseChat();
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
        currentNpcName = name;

        CharacterInfo npcInfo = GameInfo.Instance.characterInfos[name];
        string conversationKey = $"{npcInfo.name}_{npcInfo.conversationIndex}";
        Conversation conversation = GameInfo.Instance.conversationList[conversationKey];

        foreach (var button in buttons)
        {
            button.SetActive(false);
        }

        for (int i = 0; i < conversation.buttonCount; i++)
        {
            buttons[i].SetActive(true);
            switch (i)
            {
                case 0:
                    buttonTexts[i].text = conversation.answer1;
                    break;
                case 1:
                    buttonTexts[i].text = conversation.answer2;
                    break;
                case 2:
                    buttonTexts[i].text = conversation.answer3;
                    break;
                default:
                    break;
            }
          
        }
       
        chatText.text = conversation.talk;

        if (conversation.buttonCount == 0)
        {
            Invoke("CloseChat", 1f);
        }
    }


    public void ClickAnswerButton(int index)
    {
        CharacterInfo npcInfo = GameInfo.Instance.characterInfos[currentNpcName];
        string conversationKey = $"{npcInfo.name}_{npcInfo.conversationIndex}";
        Conversation conversation = GameInfo.Instance.conversationList[conversationKey];

        string next = string.Empty;

        switch (index)
        {
            case 0:
                next = conversation.next1;
                break;
            case 1:
                next = conversation.next2;
                break;
            case 2:
                next = conversation.next3;
                break;
            default:
                break;
        }

        string[] keys = next.Split('_');

        npcInfo.conversationIndex = int.Parse(keys[1]);
        SetText(keys[0]);

    }
}
