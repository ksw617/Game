using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    [SerializeField] Chat chat;

    public Transform startPoint;
    public GameObject playerPrefab;
    public PlayerController PlayerController { get; private set; }

    public GameObject npcPrefab;
    private Dictionary<string, NpcController> npcControllers;

    HashSet<int> randomNum;
    bool[] childSpawnPoint;

    private void Start()
    {
        chat = GetComponent<Chat>();

        randomNum = new HashSet<int>();
 
        npcControllers = new Dictionary<string, NpcController>();

        GameObject player = Instantiate(playerPrefab, startPoint.position, startPoint.rotation);
        this.PlayerController = player.GetComponent<PlayerController>();
        this.PlayerController.gameManager = this;

        player.transform.GetChild(GameInfo.Instance.CharacterIndex).gameObject.SetActive(true);

        childSpawnPoint = new bool[transform.childCount];
        for (int i = 0; i < childSpawnPoint.Length; i++)
        {
            childSpawnPoint[i] = false;
        }


        foreach (var npcInfo in GameInfo.Instance.npcInfos)
        {
            //위치
            int spawnPoint;
            do
            {
                spawnPoint = Random.Range(0, transform.childCount);
            }
            while (childSpawnPoint[spawnPoint]);

            childSpawnPoint[spawnPoint] = true;

            GameObject npc = Instantiate(npcPrefab, transform.GetChild(spawnPoint).position, transform.GetChild(spawnPoint).rotation);
            NpcController npcController = npc.GetComponent<NpcController>();

            //모양
            int randomIndex;
            do
            {
                randomIndex = Random.Range(0, 60);
            }
            while (randomNum.Contains(randomIndex));

            randomNum.Add(randomIndex);

            Transform childObj = npc.transform.GetChild(randomIndex);
            childObj.gameObject.SetActive(true);
            string[] words = childObj.name.Split('_');
            npc.name = $"NPC_{npcInfo.Key}";

            npcControllers.Add(npc.name, npcController);
        }
    }

    public void ConnectNPC(string npcName)
    {
        if (npcControllers.ContainsKey(npcName))
        {
            npcControllers[npcName].GoTalk(PlayerController.transform.position);
            chat.OpenChat();

            string[] names = npcName.Split('_');
           chat.SetText(names[1]);
        }

    }

    public void DisconnectNPC(string npcName)
    {
        npcControllers[npcName].StopTalk();
        chat.CloseChat();
    }
}
