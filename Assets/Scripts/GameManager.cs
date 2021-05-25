using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    [SerializeField] private Chat chat;
    [SerializeField] private ObjectPoolManager objectPoolManager;

    public Transform startPoint;
    public GameObject playerPrefab;
    public PlayerController PlayerController { get; private set; }

    [SerializeField] private Transform npcSpawnPoint;
    [SerializeField] private Transform enemySpawnPoint;


    private Dictionary<string, NpcController> npcControllers;
    HashSet<int> randomNum;


    private void Awake()
    {
        objectPoolManager = GetComponent<ObjectPoolManager>();
        chat = GetComponent<Chat>();

        npcControllers = new Dictionary<string, NpcController>();
        randomNum = new HashSet<int>();

        npcSpawnPoint = transform.GetChild(0);
        enemySpawnPoint = transform.GetChild(1);
    }

    private void Start()
    {
        CreatePlayer();
        CreateNPC();
    }

    private void CreatePlayer()
    {
        GameObject player = Instantiate(playerPrefab, startPoint.position, startPoint.rotation);
        this.PlayerController = player.GetComponent<PlayerController>();
        this.PlayerController.gameManager = this;

        player.transform.GetChild(GameInfo.Instance.CharacterIndex).gameObject.SetActive(true);
    }

    private void CreateNPC()
    {

        for (int i = 0; i < npcSpawnPoint.childCount; i++)
        {
            Transform npc = objectPoolManager.GetNPC(Random.Range(0, 60), out NpcController npcController);
            npc.position = npcSpawnPoint.GetChild(i).position;
            //npcControllers.Add(npc.name, npcController);
        }


     
    }

    public void SetNPC(string name, NpcController npcController)
    {
        npcControllers.Add(name, npcController);
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
