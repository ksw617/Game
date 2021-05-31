using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public Transform playerCamera;
    public MiniMapCamera minimapCamera;
    [SerializeField] private ObjectPoolManager objectPoolManager;

    public Transform startPoint;
    public GameObject playerPrefab;
    public PlayerController PlayerController { get; private set; }

    [SerializeField] private string currentSceneName;
    [SerializeField] private Transform npcSpawnPoint;

    private void Awake()
    {
        objectPoolManager = GetComponent<ObjectPoolManager>();
        npcSpawnPoint = transform.GetChild(0);

        currentSceneName = SceneManager.GetActiveScene().name;
    }

    private void Start()
    {
        CreatePlayer();
        CreateNPC();
    }

    private void CreatePlayer()
    {
        GameObject player = Instantiate(playerPrefab, startPoint.position, startPoint.rotation);
        minimapCamera.target = player.transform;
        this.PlayerController = player.GetComponent<PlayerController>();
        this.PlayerController.gameManager = this;

        player.transform.GetChild(GameInfo.Instance.CharacterIndex).gameObject.SetActive(true);
        playerCamera.SetParent(player.transform);
    }

    private void CreateNPC()
    {
        for (int i = 0; i < npcSpawnPoint.childCount; i++)
        {
            string key = GameInfo.Instance.mapNpcInfos[currentSceneName][i];
            Transform npc = objectPoolManager.GetCharacter(key);
            npc.position = npcSpawnPoint.GetChild(i).position;
        }
    }
}
