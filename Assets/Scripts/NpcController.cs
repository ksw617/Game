using System.Collections;
using UnityEngine;
using UnityEngine.AI;
public class NpcController : MonoBehaviour
{
    [SerializeField] private Animator animator;
    [SerializeField] private NavMeshAgent meshAgent;

    Vector3 newPosition;

    private void Awake()
    {
        animator = GetComponent<Animator>();
        meshAgent = GetComponent<NavMeshAgent>();
    }

    private void Start()
    {
        //animator.SetFloat("Y", 1f);
        FindNextPosition();
    }
    void FindNextPosition()
    {
        Vector3 randomPosition = new Vector3(Random.Range(-5f, 5f), 0f, Random.Range(-5f, 5f));
        newPosition = transform.position + randomPosition;

        meshAgent.SetDestination(newPosition);
        StartCoroutine("Move");
    }

    IEnumerator Move()
    {
        animator.SetFloat("Speed", 1f);
        while (meshAgent.destination != transform.position)
        {
            yield return new WaitForFixedUpdate();
        }

        animator.SetFloat("Speed", 0f);
        yield return new WaitForSeconds(Random.Range(5f, 10f));
        FindNextPosition();
        yield return null;
    }

    public void GoTalk(Vector3 playerPosition)
    {
        Debug.Log($"{gameObject.name} : can talk");
        newPosition = transform.position;
        meshAgent.SetDestination(newPosition);

        animator.SetFloat("Speed", 0f);
        Debug.Log(playerPosition);
        transform.LookAt(playerPosition);
        
        StopCoroutine("Move");
    }

    public void StopTalk()
    {
        Debug.Log($"{gameObject.name} : can not talk");
        FindNextPosition();
    }

}
