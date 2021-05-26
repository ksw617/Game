using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    [SerializeField] private ObjectPoolManager objectPoolManager;

    public Transform startPoint;
    public GameObject playerPrefab;
    public PlayerController PlayerController { get; private set; }


    private void Awake()
    {
        objectPoolManager = GetComponent<ObjectPoolManager>();

    }

    private void Start()
    {
        CreatePlayer();
    }

    private void CreatePlayer()
    {
        GameObject player = Instantiate(playerPrefab, startPoint.position, startPoint.rotation);
        this.PlayerController = player.GetComponent<PlayerController>();
        this.PlayerController.gameManager = this;

        player.transform.GetChild(GameInfo.Instance.CharacterIndex).gameObject.SetActive(true);
    }


}
